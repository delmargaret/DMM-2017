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
			this->lenta[i] = GRB::TS(this->lex.Lextable.table[i].lexema[0]); // на ленте только лексемы, т.е. терминалы
		// основное отличие ленты от рабочего стека Mfst в том, что на ленте хранятся только лексемы (они у нас терминалы), 
		// а в стеке хранится выражения языка Greibach, которое использует и терминалы и нетерминалы
		this->lenta_size = lex.Lextable.size;
	}
	char* Mfst::getCSt(char* buf)
	{
		MFSTSTSTACK tempst; // создаем временную переменную стека, т.к. мы не можем обратиться ко всем элементам стек сразу
		int i = 0;
		while (!st.empty()) { // из основного стека st заносим во временный стек tempst все элементы по очередиы
			buf[i++] = GRB::Rule::Chain::alphabet_to_char(st.top()); // преобразуем из short в char и записываем элемент из tempst в st
			tempst.push(st.top());
			st.pop();
		} 
		// теперь st пуст
		while (!tempst.empty())
		{
			st.push(tempst.top());
			tempst.pop();
		}
		buf[i] = 0;
		return buf;
	}
	char* Mfst::getCLenta(char* buf, short pos, short n) // печатает n символов ленты, начиная с позиции pos (в автомате это соответствует позиции lenta_position)
	{
		for (int i = 0; i < n; i++)
		{
			if (pos + i < this->lenta_size) {
				buf[i] = GRB::Rule::Chain::alphabet_to_char(this->lenta[pos + i]);
			} else {
				buf[i] = 0; // если лента закончилась, то в конец строки пишем нулевой символ и возвращаем
				return buf;
			}
		}
		buf[n] = 0;
		return buf;
	}

	char* Mfst::getDiagnosis(short n, char* buf) //получить текст для печати о диагност. сообщении
	{
		strcpy(buf, "");
		if (n < MFST_DIAGN_NUMBER) {
			int pos = diagnosis[n].lenta_position;
			if (pos >= 0) {
				int err = greibach.getRule(diagnosis[n].nrule).iderror;
				Error::ERROR er = Error::geterror(err);
				sprintf(buf, "%d: строка %d,  %s", er.id, lex.Lextable.table[pos].sn, er.message);
			}
		}
		return buf;
	}

	bool Mfst::savestate()
	{
		// создаем копию текущего состояния автомата и записываем на верхушку стека storestate
		// эта функция вызывается в том случае, если на верхушке стека st автомата Msft находится нетерминал
		this->storestate.push(MfstState(this->lenta_position, this->st, this->nrulechain, this->nrule));
#ifdef MFST_TRACE_ON
		MFST_TRACE6("SAVESTATE:", this->storestate.size())
#endif
		return true;
	}
	bool Mfst::reststate()
	{
		if (!this->storestate.empty()) {
			// восстанавливаем 3 сохраненные выше переменные из верхушки стека состояний в Msft
			this->lenta_position = this->storestate.top().lenta_position;
			this->nrulechain = this->storestate.top().nrulechain;
			this->st = this->storestate.top().st;
			// выталкиваем из стека предыдущее сохраненное состояние
			this->storestate.pop();
#ifdef MFST_TRACE_ON
			MFST_TRACE5("RESTATE")
			MFST_TRACE2
#endif
			return true; // восстановление прошло успешно
		} else 
			return false; // ничего восстановить не удалось
	}
	bool Mfst::push_chain(GRB::Rule::Chain chain)
	{
		// записываем цепочку правила в рабочий стек Msft в обратном порядке
		for (int i = chain.size - 1; i >= 0; i--) {
			this->st.push(chain.nt[i]);
		}
		return true;
	}
	Mfst::RC_STEP Mfst::step() //  основная работа автомата Mfst
	{
		if (this->st.empty() && this->lenta_position == this->lenta_size) { // кончилась лента
#ifdef MFST_TRACE_ON
			MFST_TRACE4("LENTA_END")
#endif
			return LENTA_END;
		}
		if ((this->st.empty() && this->lenta_position < this->lenta_size) || ((!this->st.empty()) && this->lenta_position >= this->lenta_size)) {
			// либо лента кончилась, и стек не пуст, либо наоборот
			return SURPRISE;
		}
		if (GRB::Rule::Chain::isN(this->st.top())) { // на вершине стека нетерминальный символ, нужно найти соответствущее правило
			GRB::Rule rule;
			GRB::Rule::Chain chain;
			int found = this->greibach.getRule(this->st.top(), rule);
			if (found == -1) { // правило не найдено
				return NS_NORULE;
			}
			this->nrule = found;
			if (cur_status == TS_NOK) { // после reststate() переходим к следующей цепочке
				this->nrulechain = rule.getNextChain(this->lenta[this->lenta_position], chain, this->nrulechain + 1); // get next chain
				if (this->nrulechain >= this->greibach.getRule(this->nrule).size) { // если цепочек больше не осталось, то делаем откат без сохранения
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
			} else { // иначе это новый нетерминальный символ и разбор начинаем с первого правила
				this->nrulechain = rule.getNextChain(this->lenta[this->lenta_position], chain, 0); // get next chain
				if (this->nrulechain == this->greibach.getRule(this->nrule).size) { //если не нашли ни одно правило начинающееся с терминала this->lenta[this->lenta_position]
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
			if (this->st.top() == this->lenta[this->lenta_position]) { // совпадает символ на верхушке стека и в текущей позиции ленты
				this->lenta_position++;
				this->st.pop();
#ifdef MFST_TRACE_ON
				MFST_TRACE3
#endif
				return TS_OK;
			} else {
				// позиция, где не совпал терминал
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
			cout << "Ошибка 305: Синтаксическая ошибка: ожидалось '" << this->getLexemDescription(expected_lexem, true) << "' но встретили '" << this->getLexemDescription(this->lex.Lextable.table[fail_position], false) << "'" << endl << endl;
			cout << "Строка " << this->lex.Lextable.table[fail_position].sn + 1 << " позиция " << this->lex.Lextable.table[fail_position].pos + 1 << endl << endl;
		} else if (this->out_of_rules_position >= 0) {
			expected_lexem.lexema[0] = GRB::Rule::Chain::alphabet_to_char(this->out_of_rules_fail_N);
			cout << "Ошибка 305: Синтаксическая ошибка: ожидалось '" << this->getLexemDescription(expected_lexem, true) << "' но встретили '" << this->getLexemDescription(this->lex.Lextable.table[this->out_of_rules_position], false) << "'" << endl << endl;
			cout << "Строка " << this->lex.Lextable.table[this->out_of_rules_position].sn + 1 << " позиция " << this->lex.Lextable.table[this->out_of_rules_position].pos + 1 << endl << endl;
		} else {
			cout << "Ошибка 305: Синтаксическая ошибка" << endl; // в теории не должна происходить
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
		//cout << setw(4) << left << "всего строк: " << this->lex.Lextable.table[this->lex.Lextable.size - 1].sn + 1 << ", синтаксический анализ выполнен без ошибок" << endl;
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

	// в этом методе формируются массивы ast->rules и ast->nrulechains
	// ast->rules -> S, N, E, F, E... - правила выыода в их порядке нахождения (нетерминалы)
	// ast->nrulechains - индексы использованных цепочек для каждого из правил в ast->rules
	// пример: если ast->rules[0] = 'S', то ast->nrulechains[0] - или 0 или 1
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
			return "заголовок функции";
			break;
		case 'N':
			return "инструкция языка";
			break;
		case 'E':
			return "выражение";
			break;
		case 'F':
			return "список параметров функции";
			break;
		case 'W':
			return "список параметров вызова функции";
			break;
		case 'M':
			return "арифметический оператор";
			break;
		case 'C':
			return "инструкция тела цикла";
			break;
		case 'l':
			if (!isExpected) {
				if (this->lex.IDtable.table[lexem.idxTI].iddatatype == IT::IDDATATYPE::INT) {
					return "целочисленный литерал";
				} else {
					return "строковый литерал";
				}
			} else {
				return "литерал";
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
					return "целочисленный тип данных";
				} else {
					return "строковый тип данных";
				}
			} else {
				return "тип данных";
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
			return "имя идентификатора";
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
		return "ничего";
	}
}