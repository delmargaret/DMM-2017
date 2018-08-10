#include "stdafx.h"
#include "AST.h"
#include "Error.h"

namespace AST {

	// метод создания корня дерева
	void Ast::createHead(GRBALPHABET startSymb) {
		head = new ASTNode;
		head->symb = startSymb;
		head->parent = NULL;
		head->child = NULL;
		head->child_count = 0;
		head->lexem = new LT::Entry('#', TI_NULLIDX, TI_NULLIDX);
	}

	// поиск имени функции, которой принадлежит node (node - инструкция)
	char* Ast::getFunctionName(ASTNode* node, Lexer::LEX* lex) {
		ASTNode* curNode = node;
		while (curNode != NULL && curNode->symb != GRB::NS('S')) {
			curNode = curNode->parent;
		}
		char* result = NULL;
		LT::Entry* temp = NULL;
		if (curNode != NULL) {
			if (curNode->chain_id == 100) {
				temp = curNode->child[2]->lexem;
			} else {
				temp = curNode->child[0]->lexem;
			}
			if (temp->lexema[0] == LEX_ID) {
				result = lex->IDtable.table[temp->idxTI].id;
			} else {
				result = "main";
			}
		}
		return result;
	}

	// получаем порядковый номер параметра node при вызове ыункции
	int Ast::getParamIdx(ASTNode* node, Lexer::LEX* lex) {
		ASTNode* curNode = node;
		int count = 0;
		while (curNode != NULL && curNode->symb != GRB::NS('E')) {
			if (curNode->symb == GRB::NS('W')) {
				count++;
			}
			curNode = curNode->parent;
		}
		return count - 1;
	}

	// поиск имени функции, которой принадлежит node во время вызова функции
	char* Ast::getFunctionCallName(ASTNode* node, Lexer::LEX* lex) {
		ASTNode* curNode = node;
		while (curNode != NULL && curNode->symb != GRB::NS('E')) {
			curNode = curNode->parent;
		}
		if (curNode != NULL) {
			return lex->IDtable.table[curNode->child[0]->lexem->idxTI].id;
		} else {
			return NULL;
		}
	}

	// проверка на совпадение типов параметров и количества параметров при вызове функции
	void Ast::checkFuncParams(ASTNode* node, Lexer::LEX* lex) {
		if (node->symb == GRB::NS('W')) {
			char* visibility = getFunctionCallName(node, lex);
			if (visibility != NULL) {
				int idx = IT::IsId(lex->IDtable, visibility, "global");
				if (idx != TI_NULLIDX) {
					int paramIdx = getParamIdx(node, lex);
					if (paramIdx >= lex->IDtable.table[idx].paramsCount) {
						throw ERROR_THROW_IN(421, node->child[0]->lexem->sn, node->child[0]->lexem->pos);
					}
					IT::Entry* func_param = lex->IDtable.table[idx].params[paramIdx];
					if (func_param->iddatatype != lex->IDtable.table[node->child[0]->lexem->idxTI].iddatatype) {
						throw ERROR_THROW_IN(419, node->child[0]->lexem->sn, node->child[0]->lexem->pos);
					}
					if (lex->IDtable.table[node->child[0]->lexem->idxTI].isArray && (!(lex->IDtable.table[idx].isLibrary && paramIdx == 0))) {
						throw ERROR_THROW_IN(418, node->child[0]->lexem->sn, node->child[0]->lexem->pos);
					}
					if ((!lex->IDtable.table[node->child[0]->lexem->idxTI].isArray) && lex->IDtable.table[idx].isLibrary && paramIdx == 0) {
						throw ERROR_THROW_IN(419, node->child[0]->lexem->sn, node->child[0]->lexem->pos);
					}
					if (node->chain_id == 100 || node->chain_id == 102) {
						if (paramIdx + 1 != lex->IDtable.table[idx].paramsCount) {
							throw ERROR_THROW_IN(422, node->child[0]->lexem->sn, node->child[0]->lexem->pos);
						}
					}
				}
			}
		}
		if (node->symb == GRB::NS('S')) {
			char* visibility = getFunctionName(node, lex);
			bool isInteger = true;
			if (strcmp(visibility, "main") != 0) {
				int idx = IT::IsId(lex->IDtable, visibility, "global");
				isInteger = lex->IDtable.table[idx].iddatatype == IT::IDDATATYPE::INT;
			}
			bool compareTo = true;
			if (node->chain_id == 100) {
				compareTo = node->child[9]->isInteger;
			} else {
				compareTo = node->child[4]->isInteger;
			}
			if (isInteger != compareTo) {
				if (node->chain_id == 100) {
					throw ERROR_THROW_IN(423, node->child[9]->lexem->sn, node->child[9]->lexem->pos);
				} else {
					throw ERROR_THROW_IN(423, node->child[4]->lexem->sn, node->child[4]->lexem->pos);
				}
			}
		}
		for (int i = 0; i < node->child_count; i++)
			checkFuncParams(node->child[i], lex);
	}

	// для каждого параметра функции, добавляет его в список параметров функции lex->IDtable.table[idx].params
	void Ast::setFuncParams(ASTNode* node, Lexer::LEX* lex) {
		if (node->symb == GRB::NS('F')) {
			char* visibility = getFunctionName(node, lex);
			if (visibility != NULL) {
				int idx = IT::IsId(lex->IDtable, visibility, "global");
				if (idx != TI_NULLIDX) {
					if (lex->IDtable.table[idx].paramsCount < TI_MAX_PARAMS_COUNT) {
						if (lex->IDtable.table[node->child[1]->lexem->idxTI].isArray) {
							throw ERROR_THROW_IN(420, node->child[1]->lexem->sn, node->child[1]->lexem->pos);
						}
						lex->IDtable.table[idx].paramsCount++;
						lex->IDtable.table[idx].params[lex->IDtable.table[idx].paramsCount - 1] = &(lex->IDtable.table[node->child[1]->lexem->idxTI]);
					} else {
						throw ERROR_THROW_IN(417, node->child[1]->lexem->sn, node->child[1]->lexem->pos);
					}
				}
			}
		}
		for (int i = 0; i < node->child_count; i++)
			setFuncParams(node->child[i], lex);
	}

	// проверка совпадения типа выражения и идентификатора
	void Ast::assignCheck(ASTNode* node, Lexer::LEX* lex) {
		if (node->symb == GRB::NS('N') || node->symb == GRB::NS('C')) {
			if ((node->symb == GRB::NS('N') && (node->chain_id == 106 || node->chain_id == 107)) || 
				((node->symb == GRB::NS('C') && (node->chain_id == 100 || node->chain_id == 101)))) {
				bool isInt = lex->IDtable.table[node->child[0]->lexem->idxTI].iddatatype == IT::IDDATATYPE::INT;
				if (isInt != node->child[2]->isInteger) {
					throw ERROR_THROW_IN(416, node->child[2]->lexem->sn, node->child[2]->lexem->pos);
				}
			}
			if (node->symb == GRB::NS('N') && (node->chain_id == 116 || node->chain_id == 117)) {
				bool isInt = lex->IDtable.table[node->child[2]->lexem->idxTI].iddatatype == IT::IDDATATYPE::INT;
				if (isInt != node->child[4]->isInteger) {
					throw ERROR_THROW_IN(416, node->child[4]->lexem->sn, node->child[4]->lexem->pos);
				}
			}
			if ((node->symb == GRB::NS('N') && (node->chain_id == 108 || node->chain_id == 109 || node->chain_id == 110 || node->chain_id == 111)) || 
				((node->symb == GRB::NS('C') && (node->chain_id == 102 || node->chain_id == 103 || node->chain_id == 104 || node->chain_id == 105)))) {
				if (!node->child[5]->isInteger) {
					throw ERROR_THROW_IN(416, node->child[5]->lexem->sn, node->child[5]->lexem->pos);
				}
			}
			if ((node->symb == GRB::NS('N') && (node->chain_id == 108 || node->chain_id == 109)) || 
				((node->symb == GRB::NS('C') && (node->chain_id == 102 || node->chain_id == 103)))) {
				if (lex->IDtable.table[node->child[0]->lexem->idxTI].value.vint <= lex->IDtable.table[node->child[2]->lexem->idxTI].value.vint) {
					throw ERROR_THROW_IN(424, node->child[2]->lexem->sn, node->child[2]->lexem->pos);
				}
			}
		}
		for (int i = 0; i < node->child_count; i++)
			assignCheck(node->child[i], lex);
	}

	// вычисление типов выражения (пример (1+2)*3) и проверка на совпадения типов внутри одного выражения
	void Ast::declExpressionType(ASTNode* node, Lexer::LEX* lex) {
		for (int i = 0; i < node->child_count; i++)
			declExpressionType(node->child[i], lex);
		if (node->symb == GRB::NS('E')) {
			if (node->parent != NULL && node->parent->symb == GRB::NS('M') && node->parent->chain_id == 103 && (node->chain_id == 104 || node->chain_id == 105)) {
				//проверяем, не является ли литерал нулем
				if (lex->IDtable.table[node->child[0]->lexem->idxTI].value.vint == 0) {
					throw ERROR_THROW_IN(406, node->child[0]->lexem->sn, node->child[0]->lexem->pos);
				}
			}
			if (node->chain_id == 102 || node->chain_id == 103 || node->chain_id == 104 || node->chain_id == 105) {
				if (lex->IDtable.table[node->child[0]->lexem->idxTI].isArray) {
					throw ERROR_THROW_IN(315, node->lexem->sn, node->lexem->pos);
				}
				node->isInteger = lex->IDtable.table[node->child[0]->lexem->idxTI].iddatatype == IT::IDDATATYPE::INT;
			}
			if (node->chain_id == 103 || node->chain_id == 105) {
				if (node->isInteger != node->child[1]->isInteger) {
					throw ERROR_THROW_IN(416, node->lexem->sn, node->lexem->pos);
				}
			}
			if (node->chain_id == 100 || node->chain_id == 101) {
				node->isInteger = node->child[1]->isInteger;
				if (node->chain_id == 100 && !node->isInteger) {
					throw ERROR_THROW_IN(414, node->lexem->sn, node->lexem->pos);
				}
			}
			if (node->chain_id == 110 || node->chain_id == 111) {
				int range = lex->IDtable.table[node->child[0]->lexem->idxTI].value.vint;
				int index = lex->IDtable.table[node->child[2]->lexem->idxTI].value.vint;
				if (index >= range) {
					throw ERROR_THROW_IN(424, node->child[2]->lexem->sn, node->child[2]->lexem->pos);
				}
			}
			if (node->chain_id == 106 || node->chain_id == 107) {
				node->isInteger = node->child[1]->isInteger;
			}
			if (node->chain_id == 108 || node->chain_id == 109) {
				node->isInteger = lex->IDtable.table[node->child[0]->lexem->idxTI].iddatatype == IT::IDDATATYPE::INT;
			}
			if (node->chain_id == 110 || node->chain_id == 111 || node->chain_id == 112 || node->chain_id == 113) {
				node->isInteger = true;
			}
		}

		if (node->symb == GRB::NS('M')) {
			node->isInteger = node->child[1]->isInteger;
			if ((!node->isInteger) && node->chain_id != 100) {
				throw ERROR_THROW_IN(414, node->lexem->sn, node->lexem->pos);
			}
		}
	}

	// проверка семантики литералов при присваивании, пример ошибки int i = "123";
	void Ast::checkLiteral(ASTNode* node, Lexer::LEX* lex) {
		if (node->lexem->lexema[0] == LEX_LITERAL) {
			ASTNode* parent = node->parent;
			if (parent != NULL && (parent->symb == GRB::NS('N') || parent->symb == GRB::NS('C'))) {
				if ((parent->symb == GRB::NS('N') && parent->chain_id == 108 || parent->chain_id == 109) || 
					(parent->symb == GRB::NS('C') && parent->chain_id == 102 || parent->chain_id == 103) ) {
					if (lex->IDtable.table[node->lexem->idxTI].iddatatype != IT::IDDATATYPE::INT) {
						throw ERROR_THROW_IN(306, node->lexem->sn, node->lexem->pos);
					}
				} else if ((parent->symb == GRB::NS('N')) && (parent->chain_id == 118 || parent->chain_id == 119)) {
					if (lex->IDtable.table[node->lexem->idxTI].iddatatype != IT::IDDATATYPE::INT) {
						throw ERROR_THROW_IN(309, node->lexem->sn, node->lexem->pos);
					}
				}
			}
		}
		for (int i = 0; i < node->child_count; i++)
			checkLiteral(node->child[i], lex);
	}

	// семантическая проверка идентификаторов и задания их типов и областей видимости
	void Ast::checkId(ASTNode* node, Lexer::LEX* lex) {
		// встретили литерал - сохраняем область видимости
		if (node->lexem->lexema[0] == LEX_LITERAL && lex->IDtable.table[node->lexem->idxTI].iddatatype == IT::IDDATATYPE::STR) {
			char* visibility = getFunctionName(node, lex);
			strcpy(lex->IDtable.table[node->lexem->idxTI].vis, visibility);
		}
		// встретили идентификатор, то проверяем семантику в соответствии с цепочками вывода
		if (node->lexem->lexema[0] == LEX_ID) {
			ASTNode* parent = node->parent;
			if (parent != NULL && (parent->symb == GRB::NS('N') || parent->symb == GRB::NS('C'))) {
				if ((parent->symb == GRB::NS('N')) && 
					(parent->chain_id == 100 || parent->chain_id == 101 || parent->chain_id == 102 || parent->chain_id == 103 || parent->chain_id == 116 || parent->chain_id == 117)) {
					if (IT::IsId(lex->IDtable, lex->IDtable.table[node->lexem->idxTI].id, "global") != TI_NULLIDX) {
						throw ERROR_THROW_IN(411, node->lexem->sn, node->lexem->pos);
					}
					char* visibility = getFunctionName(node, lex);
					if (visibility != NULL) {
						int idx = IT::IsId(lex->IDtable, lex->IDtable.table[node->lexem->idxTI].id, visibility);
						if (idx == TI_NULLIDX) {
							IT::Entry* entry = &lex->IDtable.table[node->lexem->idxTI];
							strcpy(entry->vis, visibility);
							entry->idtype = IT::IDTYPE::V;
							if (parent->chain_id == 100 || parent->chain_id == 101) {
								entry->iddatatype = IT::IDDATATYPE::INT;
								entry->isArray = true;
								if (lex->IDtable.table[parent->child[4]->lexem->idxTI].iddatatype != IT::IDDATATYPE::INT) {
									throw ERROR_THROW_IN(306, node->lexem->sn, node->lexem->pos); // ошибка, литерал должен быть целочисленный
								} else {
									entry->value.vint = lex->IDtable.table[parent->child[4]->lexem->idxTI].value.vint;
									if (entry->value.vint > 1000) {
										throw ERROR_THROW_IN(325, parent->child[4]->lexem->sn, parent->child[4]->lexem->pos);
									}
								}
							} else {
								entry->isArray = false;
								if (parent->child[1]->lexem->isIntType) {
									entry->iddatatype = IT::IDDATATYPE::INT;
								} else {
									entry->iddatatype = IT::IDDATATYPE::STR;
								}
							}
						}
						else {
							throw ERROR_THROW_IN(407, node->lexem->sn, node->lexem->pos);
						}
					} else {
						// ошибка, переменная не может быть глобальной
						// этого никогда не должно происходить, если синтаксический анализ был завершен успешно
					}
				} else if (((parent->symb == GRB::NS('N')) && (parent->chain_id == 106 || parent->chain_id == 107 || parent->chain_id == 108 || parent->chain_id == 109 || parent->chain_id == 110 || parent->chain_id == 111 
					|| parent->chain_id == 114 || parent->chain_id == 115 || parent->chain_id == 120 || parent->chain_id == 121)) || 
							((parent->symb == GRB::NS('C')))) {
					if (IT::IsId(lex->IDtable, lex->IDtable.table[node->lexem->idxTI].id, "global") != TI_NULLIDX) {
						throw ERROR_THROW_IN(411, node->lexem->sn, node->lexem->pos);
					}
					// устанавливаем корректный указатель на таблицу идентефикаторов для лексемы
					// либо ошибка использования неопределенной переменной
					if (setVariableITindex(node, lex) != NULL) {
						if (parent->chain_id == 114 || parent->chain_id == 115) {
							if (!(lex->IDtable.table[node->lexem->idxTI].idtype == IT::IDTYPE::P || 
								(lex->IDtable.table[node->lexem->idxTI].idtype == IT::IDTYPE::V && !lex->IDtable.table[node->lexem->idxTI].isArray))) {
								throw ERROR_THROW_IN(308, node->lexem->sn, node->lexem->pos);
							}
						}
						if (parent->chain_id == 120 || parent->chain_id == 121) {
							if (!(lex->IDtable.table[node->lexem->idxTI].iddatatype == IT::IDDATATYPE::INT)) {
								throw ERROR_THROW_IN(309, node->lexem->sn, node->lexem->pos);
							}
						}
					}
				}
			} else if (parent != NULL && parent->symb == GRB::NS('S')) {
				char* visibility = "global";
				int idx = IT::IsId(lex->IDtable, lex->IDtable.table[node->lexem->idxTI].id, visibility);
				if (idx == TI_NULLIDX) {
					IT::Entry* entry = &lex->IDtable.table[node->lexem->idxTI];
					strcpy(entry->vis, visibility);
					entry->idtype = IT::IDTYPE::F;
					entry->isArray = false;
					if (parent->child[0]->lexem->isIntType) {
						entry->iddatatype = IT::IDDATATYPE::INT;
					} else {
						entry->iddatatype = IT::IDDATATYPE::STR;
					}
				} else {
					throw ERROR_THROW_IN(410, node->lexem->sn, node->lexem->pos);
				}
			} else if (parent != NULL && parent->symb == GRB::NS('E')) {
				if (parent->chain_id == 102 || parent->chain_id == 103) {
					setVariableITindex(node, lex);
					if (IT::IsId(lex->IDtable, lex->IDtable.table[node->lexem->idxTI].id, "global") != TI_NULLIDX) {
						throw ERROR_THROW_IN(411, node->lexem->sn, node->lexem->pos);
					}
				} else if (parent->chain_id == 108 || parent->chain_id == 109) {
					int idx = IT::IsId(lex->IDtable, lex->IDtable.table[node->lexem->idxTI].id, "global");
					if (idx != TI_NULLIDX) {
						node->lexem->idxTI = idx;
					} else {
						throw ERROR_THROW_IN(412, node->lexem->sn, node->lexem->pos);
					} 
				} else if (parent->chain_id == 110 || parent->chain_id == 111 || parent->chain_id == 112 || parent->chain_id == 113) { 
					setVariableITindex(node, lex);
					if (IT::IsId(lex->IDtable, lex->IDtable.table[node->lexem->idxTI].id, "global") != TI_NULLIDX) {
						throw ERROR_THROW_IN(411, node->lexem->sn, node->lexem->pos);
					}
				}
			} else if (parent != NULL && parent->symb == GRB::NS('F')) {
				if (IT::IsId(lex->IDtable, lex->IDtable.table[node->lexem->idxTI].id, "global") != TI_NULLIDX) {
					throw ERROR_THROW_IN(411, node->lexem->sn, node->lexem->pos);
				}
				char* visibility = getFunctionName(node, lex);
				if (visibility != NULL) {
					int idx = IT::IsId(lex->IDtable, lex->IDtable.table[node->lexem->idxTI].id, visibility);
					if (idx == TI_NULLIDX) {
						IT::Entry* entry = &lex->IDtable.table[node->lexem->idxTI];
						strcpy(entry->vis, visibility);
						entry->idtype = IT::IDTYPE::P;
						entry->isArray = false;
						if (parent->child[0]->lexem->isIntType) {
							entry->iddatatype = IT::IDDATATYPE::INT;
						} else {
							entry->iddatatype = IT::IDDATATYPE::STR;
						}
					} else {
						throw ERROR_THROW_IN(413, node->lexem->sn, node->lexem->pos);
					}
					if (IT::IsId(lex->IDtable, lex->IDtable.table[node->lexem->idxTI].id, "global") != TI_NULLIDX) {
						throw ERROR_THROW_IN(411, node->lexem->sn, node->lexem->pos);
					}
				}
			}  else if (parent != NULL && parent->symb == GRB::NS('W')) {
				char* visibility = setVariableITindex(node, lex);
				if (visibility != NULL) {
					int idx = IT::IsId(lex->IDtable, lex->IDtable.table[node->lexem->idxTI].id, visibility);
					if (idx == TI_NULLIDX) {
						throw ERROR_THROW_IN(214, node->lexem->sn, node->lexem->pos);
					}
					if (IT::IsId(lex->IDtable, lex->IDtable.table[node->lexem->idxTI].id, "global") != TI_NULLIDX) {
						throw ERROR_THROW_IN(411, node->lexem->sn, node->lexem->pos);
					}
				}
			}
		}
		for (int i = 0; i < node->child_count; i++)
			checkId(node->child[i], lex);
	}

	char* Ast::setVariableITindex(ASTNode* node, Lexer::LEX* lex) {
		char* visibility = getFunctionName(node, lex);
		if (visibility != NULL) {
			int idx = IT::IsId(lex->IDtable, lex->IDtable.table[node->lexem->idxTI].id, visibility);
			if (idx == TI_NULLIDX) {
				throw ERROR_THROW_IN(214, node->lexem->sn, node->lexem->pos);
			} else {
				node->lexem->idxTI = idx;
			}
		}
		return visibility;
	}

	// основной метод построения полного синтаксического дерева разбора с его одновременным обходом в глубину
	void Ast::buildAST(int* rule_id, int* lex_id, ASTNode* node, GRB::Rule* rules, short* nrulechains, Lexer::LEX* lex) {
		if (GRB::Rule::Chain::isT(node->symb)) { // если текущая вершина терминал, то просто сохраняем ссылку на лексему, соответствующую данному терминалу
			node->lexem = &(lex->Lextable.table[(*lex_id)++]);
			return;
		}
		// в случае, если текущая вершина это нетерминал, то нужно сохранить данные о правиле вывода и обойти поддеревья, соответствующие дочерним вершинам
		node->lexem = new LT::Entry('#', TI_NULLIDX, TI_NULLIDX);
		GRB::Rule::Chain chain = rules[*rule_id].chains[nrulechains[*rule_id]];
		int chain_size = chain.size;
		node->chain_id = chain.chain_id;
		(*rule_id)++;
		node->child = new ASTNode*[chain_size]; // создание дочерних вершин для текущей вершины
		node->child_count = chain_size;
		for (int i = 0; i < chain_size; i++) { // обход дочерних вершин
			node->child[i] = new ASTNode;
			node->child[i]->symb = chain.nt[i];
			node->child[i]->parent = node;
			node->child[i]->child_count = 0;
			node->child[i]->child = NULL;
			buildAST(rule_id, lex_id, node->child[i], rules, nrulechains, lex); 
		}
		node->lexem->lexemId = node->child[0]->lexem->lexemId;
		node->lexem->sn = node->child[0]->lexem->sn; // для нетерминала, первый ребенок это всегда терминал (см. правила грамматики грейбах)
		node->lexem->pos = node->child[0]->lexem->pos; // сохраняем позицию строки и номера символа в строке
	}

	void Ast::printAST(ASTNode* node, ofstream& AstOutput) {
		if (GRB::Rule::Chain::isN(node->symb)) {
			AstOutput << GRB::Rule::Chain::alphabet_to_char(node->symb) << " ";
		} else {
			AstOutput << node->lexem->lexema[0] << " ";
		}
		for (int i = 0; i < node->child_count; i++)
			printAST(node->child[i], AstOutput);
	}
}