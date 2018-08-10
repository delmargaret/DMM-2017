#include "stdafx.h"
#include "MFST.h"
#include "GRB.h"
#include "Error.h"
#include "Trace.h"

namespace MFST
{
	ofstream mfstout;

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain, int nrule)
	{
		this->lenta_position = pposition;
		this->nrulechain = pnrulechain;
		this->st = pst;
		this->nrule = nrule;
	}

	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
	{
		this->lenta_position = plenta_position;
		this->rc_step = prc_step;
		this->nrule = pnrule;
		this->nrule_chain = pnrule_chain;
	}

	Mfst::Mfst(Lexer::LEX plex, GRB::Greibach pgreibach)
	{
		this->lex = plex;
		this->greibach = pgreibach;
		this->lenta = new GRBALPHABET[this->lex.Lextable.size];
		for (int i = 0; i < this->lex.Lextable.size; i++)
			this->lenta[i] = GRB::TS(this->lex.Lextable.table[i].lexema[0]); // �� ����� ������ �������, �.�. ���������
		// �������� ������� ����� �� �������� ����� Mfst � ���, ��� �� ����� �������� ������ ������� (��� � ��� ���������), 
		// � � ����� �������� ��������� ����� Greibach, ������� ���������� � ��������� � �����������
		this->lenta_size = lex.Lextable.size;
	}
	char* Mfst::getCSt(char* buf)
	{
		MFSTSTSTACK tempst; // ������� ��������� ���������� �����, �.�. �� �� ����� ���������� �� ���� ��������� ���� �����
		int i = 0;
		while (!st.empty()) { // �� ��������� ����� st ������� �� ��������� ���� tempst ��� �������� �� ��������
			buf[i++] = GRB::Rule::Chain::alphabet_to_char(st.top()); // ����������� �� short � char � ���������� ������� �� tempst � st
			tempst.push(st.top());
			st.pop();
		} 
		// ������ st ����
		while (!tempst.empty())
		{
			st.push(tempst.top());
			tempst.pop();
		}
		buf[i] = 0;
		return buf;
	}
	char* Mfst::getCLenta(char* buf, short pos, short n) // �������� n �������� �����, ������� � ������� pos (� �������� ��� ������������� ������� lenta_position)
	{
		for (int i = 0; i < n; i++)
		{
			if (pos + i < this->lenta_size) {
				buf[i] = GRB::Rule::Chain::alphabet_to_char(this->lenta[pos + i]);
			} else {
				buf[i] = 0; // ���� ����� �����������, �� � ����� ������ ����� ������� ������ � ����������
				return buf;
			}
		}
		buf[n] = 0;
		return buf;
	}

	char* Mfst::getDiagnosis(short n, char* buf) //�������� ����� ��� ������ � ��������. ���������
	{
		strcpy(buf, "");
		if (n < MFST_DIAGN_NUMBER) {
			int pos = diagnosis[n].lenta_position;
			if (pos >= 0) {
				int err = greibach.getRule(diagnosis[n].nrule).iderror;
				Error::ERROR er = Error::geterror(err);
				sprintf(buf, "%d: ������ %d,  %s", er.id, lex.Lextable.table[pos].sn, er.message);
			}
		}
		return buf;
	}

	bool Mfst::savestate()
	{
		// ������� ����� �������� ��������� �������� � ���������� �� �������� ����� storestate
		// ��� ������� ���������� � ��� ������, ���� �� �������� ����� st �������� Msft ��������� ����������
		this->storestate.push(MfstState(this->lenta_position, this->st, this->nrulechain, this->nrule));
#ifdef MFST_TRACE_ON
		MFST_TRACE6("SAVESTATE:", this->storestate.size())
#endif
		return true;
	}
	bool Mfst::reststate()
	{
		if (!this->storestate.empty()) {
			// ��������������� 3 ����������� ���� ���������� �� �������� ����� ��������� � Msft
			this->lenta_position = this->storestate.top().lenta_position;
			this->nrulechain = this->storestate.top().nrulechain;
			this->st = this->storestate.top().st;
			// ����������� �� ����� ���������� ����������� ���������
			this->storestate.pop();
#ifdef MFST_TRACE_ON
			MFST_TRACE5("RESTATE")
			MFST_TRACE2
#endif
			return true; // �������������� ������ �������
		} else 
			return false; // ������ ������������ �� �������
	}
	bool Mfst::push_chain(GRB::Rule::Chain chain)
	{
		// ���������� ������� ������� � ������� ���� Msft � �������� �������
		for (int i = chain.size - 1; i >= 0; i--) {
			this->st.push(chain.nt[i]);
		}
		return true;
	}
	Mfst::RC_STEP Mfst::step() //  �������� ������ �������� Mfst
	{
		if (this->st.empty() && this->lenta_position == this->lenta_size) { // ��������� �����
#ifdef MFST_TRACE_ON
			MFST_TRACE4("LENTA_END")
#endif
			return LENTA_END;
		}
		if ((this->st.empty() && this->lenta_position < this->lenta_size) || ((!this->st.empty()) && this->lenta_position >= this->lenta_size)) {
			// ���� ����� ���������, � ���� �� ����, ���� ��������
			return SURPRISE;
		}
		if (GRB::Rule::Chain::isN(this->st.top())) { // �� ������� ����� �������������� ������, ����� ����� �������������� �������
			GRB::Rule rule;
			GRB::Rule::Chain chain;
			int found = this->greibach.getRule(this->st.top(), rule);
			if (found == -1) { // ������� �� �������
				return NS_NORULE;
			}
			this->nrule = found;
			if (cur_status == TS_NOK) { // ����� reststate() ��������� � ��������� �������
				this->nrulechain = rule.getNextChain(this->lenta[this->lenta_position], chain, this->nrulechain + 1); // get next chain
				if (this->nrulechain >= this->greibach.getRule(this->nrule).size) { // ���� ������� ������ �� ��������, �� ������ ����� ��� ����������
#ifdef MFST_TRACE_ON
					MFST_TRACE4("NS_NORULECHAIN/NS_NORULE")
#endif
					if (this->out_of_rules_position < this->lenta_position) {
						this->out_of_rules_position = this->lenta_position;
						this->out_of_rules_fail_N = this->st.top();
						this->out_of_rules_fail_T = this->lenta[this->lenta_position];
					}
					this->savediagnosis(NS_NORULECHAIN);
					if (!this->reststate())	{
						return NS_NORULECHAIN;
					}
					return TS_NOK;
				}
			} else { // ����� ��� ����� �������������� ������ � ������ �������� � ������� �������
				this->nrulechain = rule.getNextChain(this->lenta[this->lenta_position], chain, 0); // get next chain
				if (this->nrulechain == this->greibach.getRule(this->nrule).size) { //���� �� ����� �� ���� ������� ������������ � ��������� this->lenta[this->lenta_position]
					this->nrulechain = 0;
					return TS_NOK;
				}
			}
#ifdef MFST_TRACE_ON
			MFST_TRACE1
#endif
			this->savestate();
			this->st.pop();
			this->push_chain(rule.chains[this->nrulechain]);
#ifdef MFST_TRACE_ON
			MFST_TRACE2
#endif
			return NS_OK;
		} else {
			if (this->st.top() == this->lenta[this->lenta_position]) { // ��������� ������ �� �������� ����� � � ������� ������� �����
				this->lenta_position++;
				this->st.pop();
#ifdef MFST_TRACE_ON
				MFST_TRACE3
#endif
				return TS_OK;
			} else {
				// �������, ��� �� ������ ��������
				if (this->fail_position < this->lenta_position) {
					this->fail_position = this->lenta_position;
					this->fail_symbol = this->lenta[this->lenta_position];
					this->expected_symbol = st.top();
				}
#ifdef MFST_TRACE_ON
				MFST_TRACE4("TS_NOK/NS_NORULECHAIN")
#endif
				if (!this->reststate()) {
					return NS_NORULECHAIN;
				}
				return TS_NOK;
			}
		}
	}

	void Mfst::printDiagnosis()
	{
		/*char buf[1024];
		for (int i = 0; i < MFST_DIAGN_NUMBER; i++)
			if (this->diagnosis[i].lenta_position >= 0)
				cout << getDiagnosis(i, buf) << endl;*/
		printError();
	}

	void Mfst::printError()
	{
		LT::Entry expected_lexem;
		if (this->fail_position >= 0) {
			expected_lexem.lexema[0] = GRB::Rule::Chain::alphabet_to_char(expected_symbol);
			cout << "������ 305: �������������� ������: ��������� '" << this->getLexemDescription(expected_lexem, true) << "' �� ��������� '" << this->getLexemDescription(this->lex.Lextable.table[fail_position], false) << "'" << endl << endl;
			cout << "������ " << this->lex.Lextable.table[fail_position].sn + 1 << " ������� " << this->lex.Lextable.table[fail_position].pos + 1 << endl << endl;
		} else if (this->out_of_rules_position >= 0) {
			expected_lexem.lexema[0] = GRB::Rule::Chain::alphabet_to_char(this->out_of_rules_fail_N);
			cout << "������ 305: �������������� ������: ��������� '" << this->getLexemDescription(expected_lexem, true) << "' �� ��������� '" << this->getLexemDescription(this->lex.Lextable.table[this->out_of_rules_position], false) << "'" << endl << endl;
			cout << "������ " << this->lex.Lextable.table[this->out_of_rules_position].sn + 1 << " ������� " << this->lex.Lextable.table[this->out_of_rules_position].pos + 1 << endl << endl;
		} else {
			cout << "������ 305: �������������� ������" << endl; // � ������ �� ������ �����������
		}
	}

	bool Mfst::start()
	{
		mfstout.open("MFST.txt", std::fstream::out);
#ifdef MFST_TRACE_ON
		MFST_TRACE_START
#endif
		this->st.push(this->greibach.startN);

		this->lenta_position = 0;
		this->lenta_size = this->lex.Lextable.size;
		this->nrule = 0;
		this->nrulechain = 0;

		this->fail_position = -1;
		this->expected_symbol = 0;
		this->fail_symbol = 0;

		this->out_of_rules_position = -1;
		this->out_of_rules_fail_N = 0;
		this->out_of_rules_fail_T = 0;

		for (int i = 0; i < MFST_DIAGN_NUMBER; i++)
			diagnosis[i].lenta_position = -1;

		cur_status = Mfst::RC_STEP::NS_OK;
		while (cur_status != Mfst::RC_STEP::LENTA_END)
		{
			cur_status = this->step();
			switch (cur_status)
			{
			case Mfst::RC_STEP::NS_NORULE: 
#ifdef MFST_TRACE_ON
				MFST_TRACE4("------>NS_NORULE")
#endif
				//this->printDiagnosis();
				mfstout.close();
				return false;
				break;
			case Mfst::RC_STEP::NS_NORULECHAIN:
#ifdef MFST_TRACE_ON
				MFST_TRACE4("------>NS_NORULECHAIN")
#endif
				//this->printDiagnosis();
				return false;
				mfstout.close();
				break;
			case Mfst::RC_STEP::NS_ERROR:
#ifdef MFST_TRACE_ON
				MFST_TRACE4("------>NS_ERROR")
#endif
				//this->printDiagnosis();
				return false;
				mfstout.close();
				break;
			case Mfst::RC_STEP::SURPRISE:
#ifdef MFST_TRACE_ON
				MFST_TRACE4("------>NS_SURPRISE")
#endif
				//this->printDiagnosis();
				return false;
				mfstout.close();
				break;
			}
		}
#ifdef MFST_TRACE_ON
		MFST_TRACE4("------>LENTA_END")
		cout << "-------------------------------------------------------------------------------" << endl;
#endif
		//cout << setw(4) << left << "����� �����: " << this->lex.Lextable.table[this->lex.Lextable.size - 1].sn + 1 << ", �������������� ������ �������� ��� ������" << endl;
		mfstout.close();
		return true;
	}

	bool Mfst::savediagnosis(RC_STEP pprc_step)
	{
		int i = 0;
		while (i < MFST_DIAGN_NUMBER && this->lenta_position <= diagnosis[i].lenta_position)
			i++;
		if (i < MFST_DIAGN_NUMBER) {
			diagnosis[i] = Mfst::MfstDiagnosis(this->lenta_position, pprc_step, this->nrule, this->nrulechain);
			for (int j = i + 1; j < MFST_DIAGN_NUMBER; j++)
				diagnosis[j].lenta_position = -1;
			return true;
		}
		else
			return false;
		return true;
	}

	// � ���� ������ ����������� ������� ast->rules � ast->nrulechains
	// ast->rules -> S, N, E, F, E... - ������� ������ � �� ������� ���������� (�����������)
	// ast->nrulechains - ������� �������������� ������� ��� ������� �� ������ � ast->rules
	// ������: ���� ast->rules[0] = 'S', �� ast->nrulechains[0] - ��� 0 ��� 1
	void Mfst::prepareAstLexems(AST::Ast* ast) {
		MfstState state;
		GRB::Rule rule;
		std::stack<MfstState> tempstore;
		while (!storestate.empty()) {
			tempstore.push(storestate.top());
			storestate.pop();
		}
		ast->rules = new GRB::Rule[tempstore.size()];
		ast->nrulechains = new short[tempstore.size()];
		int i = 0;
		while (!tempstore.empty()) {
			state = tempstore.top();
			rule = this->greibach.getRule(state.nrule);
			ast->rules[i] = rule;
			ast->nrulechains[i] = state.nrulechain;
			i++;
			storestate.push(tempstore.top());
			tempstore.pop();
		}
		ast->rules_size = i;
	}

	void Mfst::printRules()
	{
		//mfstout.open("MFST.txt", std::fstream::out);
		MfstState state;
		GRB::Rule rule;
		std::stack<MfstState> tempstore;
		while (!storestate.empty()) {
			tempstore.push(storestate.top());
			storestate.pop();
		}
		while (!tempstore.empty()) {
			state = tempstore.top();
			rule = this->greibach.getRule(state.nrule);
//#ifdef MFST_TRACE_ON
			MFST_TRACE7
//#endif
			storestate.push(tempstore.top());
			tempstore.pop();
		}
	}

	char* Mfst::getLexemDescription(LT::Entry lexem, bool isExpected) {
		switch (lexem.lexema[0]) {
		case 'S':
			return "��������� �������";
			break;
		case 'N':
			return "���������� �����";
			break;
		case 'E':
			return "���������";
			break;
		case 'F':
			return "������ ���������� �������";
			break;
		case 'W':
			return "������ ���������� ������ �������";
			break;
		case 'M':
			return "�������������� ��������";
			break;
		case 'C':
			return "���������� ���� �����";
			break;
		case 'l':
			if (!isExpected) {
				if (this->lex.IDtable.table[lexem.idxTI].iddatatype == IT::IDDATATYPE::INT) {
					return "������������� �������";
				} else {
					return "��������� �������";
				}
			} else {
				return "�������";
			}
			break;
		case 'm':
			return "main";
			break;
		case 'c':
			return "for";
			break;
		case 't':
			if (!isExpected) {
				if (lexem.isIntType) {
					return "������������� ��� ������";
				} else {
					return "��������� ��� ������";
				}
			} else {
				return "��� ������";
			}
			break;
		case 'f':
			return "function";
			break;
		case 'r':
			return "ret";
			break;
		case 'd':
			return "declare";
			break;
		case 'p':
			return "print";
			break;
		case 'a':
			return "massiv";
			break;
		case 'i':
			return "��� ��������������";
			break;
		case '+':
			return "+";
			break;
		case '-':
			return "-";
			break;
		case '*':
			return "*";
			break;
		case '/':
			return "/";
			break;
		case ';':
			return ";";
			break;
		case '=':
			return "=";
			break;
		case ',':
			return ",";
			break;
		case '{':
			return "{";
			break;
		case '}':
			return "}";
			break;
		case '(':
			return "(";
			break;
		case ')':
			return ")";
			break;
		case '[':
			return "[";
			break;
		case ']':
			return "]";
			break;
		}
		return "������";
	}
}