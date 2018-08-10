#include "stdafx.h"
#include "GenCode.h"

using namespace std;

namespace GENCODE {

	// генерация параметров функции, при определении ее заголовка
	void GenCode::generateParams(std::stringstream* instructions, std::stringstream* locals, AST::ASTNode* node, Lexer::LEX* lex) {
		IT::Entry* entry = &(lex->IDtable.table[node->child[1]->lexem->idxTI]);
		*instructions << entry->id << ":";
		if (entry->iddatatype == IT::IDDATATYPE::INT) {
			*instructions << "SDWORD";
		} else {
			*instructions << "DWORD";
		}
		if (node->chain_id == 101) {
			*instructions << ",";
			generateParams(instructions, locals, node->child[3], lex);
		}
	}

	GenCode::ComputeNode* GenCode::factor(AST::ASTNode** node, Lexer::LEX* lex) {
		ComputeNode* res = new ComputeNode();
		AST::ASTNode* curNode = *node;
		if (curNode->symb == GRB::NS('E')) {
			if (curNode->chain_id == 100 || curNode->chain_id == 101) {
				res->unary = true;
				if (curNode->chain_id == 100) {
					res->operation = '-';
				} else {
					res->operation = '+';
				}
				*node = curNode->child[1];
				res->left = expr(node, lex);
				if (res->left->left != NULL) {
					ComputeNode* nodeLookup = res->left;
					while (nodeLookup->left != NULL && (!(nodeLookup->left->node->chain_id == 106 && nodeLookup->left->node->chain_id == 107)) && ((nodeLookup->operation == '+' || nodeLookup->operation == '-') && (!nodeLookup->unary))) {
					//while (nodeLookup->left != NULL && nodeLookup->left->node->symb == GRB::NS('M')) {
						nodeLookup = nodeLookup->left;
					}
					nodeLookup->left = new ComputeNode(NULL, nodeLookup->left, NULL, curNode, res->operation, true);
					res = res->left;
				} else {
					res->node = curNode;
				}
			}
			if (curNode->chain_id == 106 || curNode->chain_id == 107) {
				*node = curNode->child[1];
				res = expr(node, lex);
				*node = curNode->child[curNode->child_count - 1];
				res->node = curNode;
			}
			if (curNode->chain_id == 102 || curNode->chain_id == 103 || curNode->chain_id == 104 || curNode->chain_id == 105 || 
				curNode->chain_id == 108 || curNode->chain_id == 109 || curNode->chain_id == 110 || curNode->chain_id == 111 || 
				curNode->chain_id == 112 || curNode->chain_id == 113) {
				res->val = curNode;
				res->node = curNode;
				*node = (*node)->child[(*node)->child_count - 1];
			}
		}
		return res;
	}
	GenCode::ComputeNode* GenCode::term(AST::ASTNode** node, Lexer::LEX* lex) {
		ComputeNode* res = factor(node, lex);
		while ((*node)->symb == GRB::NS('M') && ((*node)->chain_id == 102 || (*node)->chain_id == 103)) {
			AST::ASTNode* curNode = *node;
			if ((*node)->chain_id == 102) {
				*node = (*node)->child[1];
				res = new ComputeNode(NULL, res, factor(node, lex), curNode, '*', false);
			} else if ((*node)->chain_id == 103) {
				*node = (*node)->child[1];
				res = new ComputeNode(NULL, res, factor(node, lex), curNode, '/', false);
			}
		}
		return res;
	}
	GenCode::ComputeNode* GenCode::expr(AST::ASTNode** node, Lexer::LEX* lex) {
		ComputeNode* res = term(node, lex);
		while ((*node)->symb == GRB::NS('M') && ((*node)->chain_id == 100 || (*node)->chain_id == 101)) {
			AST::ASTNode* curNode = *node;
			if ((*node)->chain_id == 100) {
				*node = (*node)->child[1];
				res = new ComputeNode(NULL, res, term(node, lex), curNode, '+', false);
			} else if ((*node)->chain_id == 101) {
				*node = (*node)->child[1];
				res = new ComputeNode(NULL, res, term(node, lex), curNode, '-', false);
			}
		}
		return res;
	}

	void GenCode::computeExpression(std::stringstream* instructions, std::stringstream* locals, GenCode::ComputeNode* node, Lexer::LEX* lex) {
		if (node->val == NULL) {
			if (node->unary == false) {
				computeExpression(instructions, locals, node->left, lex);
				computeExpression(instructions, locals, node->right, lex);
				if (node->operation == '+') { //+
					if (node->node->isInteger) {
						*instructions << "POP ebx" << endl;
						*instructions << "POP eax" << endl;
						*instructions << "ADD eax, ebx" << endl;
						*instructions << "PUSH eax" << endl;
					} else {
						*instructions << "PUSH ecx" << endl;
						*instructions << "invoke lstrcat, addr S2, addr S1" << endl;
						*instructions << "invoke lstrcpy, addr S1, addr S2" << endl;
						*instructions << "POP ecx" << endl;
					}
				} else if (node->operation == '-') { //-
					*instructions << "POP ebx" << endl;
					*instructions << "POP eax" << endl;
					*instructions << "SUB eax, ebx" << endl;
					*instructions << "PUSH eax" << endl;
				} else if (node->operation == '*') { //*
					*instructions << "POP ebx" << endl;
					*instructions << "POP eax" << endl;
					*instructions << "XOR EDX, EDX" << endl;
					*instructions << "IMUL ebx" << endl;
					*instructions << "PUSH eax" << endl;
				} if (node->operation == '/') { // /
					*instructions << "POP ebx" << endl;
					*instructions << "POP eax" << endl;
					*instructions << "XOR EDX, EDX" << endl;
					*instructions << "IDIV ebx" << endl;
					*instructions << "PUSH eax" << endl;
				}
			} else {
				computeExpression(instructions, locals, node->left, lex);
				*instructions << "POP eax" << endl;
				if (node->operation == '-') {
					*instructions << "NEG eax" << endl;
				}
				*instructions << "PUSH eax" << endl;
			}
		} else {
			if (node->val->chain_id == 102 || node->val->chain_id == 103 || node->val->chain_id == 104 || node->val->chain_id == 105) {
				IT::Entry* entry = &(lex->IDtable.table[node->val->child[0]->lexem->idxTI]);
				if (entry->iddatatype == IT::IDDATATYPE::INT) {
					if (node->val->chain_id == 104 ||node->val->chain_id == 105) {
						*instructions << "PUSH " << entry->value.vint << endl;
					} else {
						*instructions << "PUSH " << entry->id << endl;
					}
				} else {
					*instructions << "PUSH ecx" << endl;
					*instructions << "invoke lstrcpy, addr S2, addr S1" << endl;
					if (node->val->chain_id == 104 || node->val->chain_id == 105) {
						*instructions << "invoke lstrcpy, addr S1, offset " << getConstName(node->val->child[0], lex) << endl;
					} else {
						if (entry->idtype == IT::IDTYPE::P) {
							*instructions << "invoke lstrcpy, addr S1, " << entry->id << endl;
						} else {
							*instructions << "invoke lstrcpy, addr S1, addr " << entry->id << endl;
						}
					}
					*instructions << "POP ecx" << endl;
				}
			}
			if (node->val->chain_id == 110 || node->val->chain_id == 111 || node->val->chain_id == 112 || node->val->chain_id == 113) {
				IT::Entry* entry = &(lex->IDtable.table[node->val->child[0]->lexem->idxTI]);
				IT::Entry* index = &(lex->IDtable.table[node->val->child[2]->lexem->idxTI]);
				if (node->val->chain_id == 110 || node->val->chain_id == 111) {
					*instructions << "PUSH " << entry->id << "[" << index->value.vint << "*4]" << endl;
				} else {
					*instructions << "XOR edx, edx" << endl;
					*instructions << "MOV eax, " << index->id << endl;
					*instructions << "PUSH ebx" << endl;
					*instructions << "MOV ebx, 4" << endl;
					*instructions << "MUL ebx"<< endl;
					*instructions << "POP ebx" << endl;
					*instructions << "PUSH " << entry->id << "[eax]" << endl;
				}
			}
			if (node->val->chain_id == 108 || node->val->chain_id == 109) {
				IT::Entry* entry = &(lex->IDtable.table[node->val->child[0]->lexem->idxTI]);
				char* functionName = AST::Ast::getFunctionName(node->val, lex);
				int idx = IT::IsId(lex->IDtable, functionName, "global");
				int usedP = 0;
				int maxP = lex->IDtable.table[idx].createdTempP;
				std::string s;
				if (idx != TI_NULLIDX) {
					s = generateInvokeParams(instructions, locals, node->val->child[2], lex, maxP, usedP);
				} else {
					maxP = createdTempP;
					s = generateInvokeParams(instructions, locals, node->val->child[2], lex, maxP, usedP);
				}
				if (usedP > maxP) {
					if (idx != TI_NULLIDX) {
						lex->IDtable.table[idx].createdTempP = usedP;
					} else {
						createdTempP = usedP;
					}
				}
				*instructions << "INVOKE " << entry->id << "," << s << endl;
				if (entry->iddatatype == IT::IDDATATYPE::INT) {
					*instructions << "PUSH eax" << endl;
				} else {
					*instructions << "PUSH ecx" << endl;
					*instructions << "PUSH eax" << endl;
					*instructions << "invoke lstrcpy, addr S2, addr S1" << endl;
					*instructions << "POP eax" << endl;
					*instructions << "POP ecx" << endl;
					*instructions << "PUSH ecx" << endl;
					*instructions << "invoke lstrcpy, addr S1, eax" << endl;
					*instructions << "POP ecx" << endl;
				}
			}
		}
	}

	// генерация кода для вычисления выражений (например (1+2)*3 или "123" + "456")
	void GenCode::computeExpression(std::stringstream* instructions, std::stringstream* locals, AST::ASTNode* node, Lexer::LEX* lex) {
		computeExpression(instructions, locals, expr(&node, lex), lex);
		return;
		if (node->symb == GRB::NS('E')) {
			if (node->chain_id == 102 || node->chain_id == 103 || node->chain_id == 104 || node->chain_id == 105) {
				IT::Entry* entry = &(lex->IDtable.table[node->child[0]->lexem->idxTI]);
				if (entry->iddatatype == IT::IDDATATYPE::INT) {
					if (node->chain_id == 104 || node->chain_id == 105) {
						*instructions << "PUSH " << entry->value.vint << endl;
					} else {
						*instructions << "PUSH " << entry->id << endl;
					}
				} else {
					*instructions << "PUSH ecx" << endl;
					*instructions << "invoke lstrcpy, addr S2, addr S1" << endl;
					if (node->chain_id == 104 || node->chain_id == 105) {
						*instructions << "invoke lstrcpy, addr S1, offset " << getConstName(node->child[0], lex) << endl;
					} else {
						if (entry->idtype == IT::IDTYPE::P) {
							*instructions << "invoke lstrcpy, addr S1, " << entry->id << endl;
						} else {
							*instructions << "invoke lstrcpy, addr S1, addr " << entry->id << endl;
						}
					}
					*instructions << "POP ecx" << endl;
				}
			}
			if (node->chain_id == 110 || node->chain_id == 111 || node->chain_id == 112 || node->chain_id == 113) {
				IT::Entry* entry = &(lex->IDtable.table[node->child[0]->lexem->idxTI]);
				IT::Entry* index = &(lex->IDtable.table[node->child[2]->lexem->idxTI]);
				if (node->chain_id == 110 || node->chain_id == 111) {
					*instructions << "PUSH " << entry->id << "[" << index->value.vint << "*4]" << endl;
				} else {
					*instructions << "XOR edx, edx" << endl;
					*instructions << "MOV eax, " << index->id << endl;
					*instructions << "PUSH ebx" << endl;
					*instructions << "MOV ebx, 4" << endl;
					*instructions << "MUL ebx"<< endl;
					*instructions << "POP ebx" << endl;
					*instructions << "PUSH " << entry->id << "[eax]" << endl;
				}
			}
			if (node->chain_id == 106 || node->chain_id == 107) { // (E)
				computeExpression(instructions, locals, node->child[1], lex);
			}
			if (node->chain_id == 100 || node->chain_id == 101) { // +E -E
				computeExpression(instructions, locals, node->child[1], lex);
				*instructions << "POP eax" << endl;
				if (node->chain_id == 100) {
					*instructions << "NEG eax" << endl;
				}
				*instructions << "PUSH eax" << endl;
			}
			if (node->chain_id == 108 || node->chain_id == 109) {
				IT::Entry* entry = &(lex->IDtable.table[node->child[0]->lexem->idxTI]);
				char* functionName = AST::Ast::getFunctionName(node, lex);
				int idx = IT::IsId(lex->IDtable, functionName, "global");
				int usedP = 0;
				int maxP = lex->IDtable.table[idx].createdTempP;
				std::string s;
				if (idx != TI_NULLIDX) {
					s = generateInvokeParams(instructions, locals, node->child[2], lex, maxP, usedP);
				} else {
					maxP = createdTempP;
					s = generateInvokeParams(instructions, locals, node->child[2], lex, maxP, usedP);
				}
				if (usedP > maxP) {
					if (idx != TI_NULLIDX) {
						lex->IDtable.table[idx].createdTempP = usedP;
					} else {
						createdTempP = usedP;
					}
				}
				*instructions << "INVOKE " << entry->id << "," << s << endl;
				if (entry->iddatatype == IT::IDDATATYPE::INT) {
					*instructions << "PUSH eax" << endl;
				} else {
					*instructions << "PUSH ecx" << endl;
					*instructions << "PUSH eax" << endl;
					*instructions << "invoke lstrcpy, addr S2, addr S1" << endl;
					*instructions << "POP eax" << endl;
					*instructions << "POP ecx" << endl;
					*instructions << "PUSH ecx" << endl;
					*instructions << "invoke lstrcpy, addr S1, eax" << endl;
					*instructions << "POP ecx" << endl;
				}
			}
		}
		if (node->child[node->child_count - 1]->symb == GRB::NS('M')) {
			int child_chain_id = node->child[node->child_count - 1]->chain_id;
			if (child_chain_id == 100) { //+
				if (node->child[1]->isInteger) {
					computeExpression(instructions, locals, node->child[node->child_count - 1], lex);
					*instructions << "POP ebx" << endl;
					*instructions << "POP eax" << endl;
					*instructions << "ADD eax, ebx" << endl;
					*instructions << "PUSH eax" << endl;
				} else {
					*instructions << "PUSH ebx" << endl;
					computeExpression(instructions, locals, node->child[node->child_count - 1], lex);
					*instructions << "POP ebx" << endl;

					*instructions << "PUSH ecx" << endl;
					*instructions << "invoke lstrcat, addr S2, addr S1" << endl;
					*instructions << "invoke lstrcpy, addr S1, addr S2" << endl;
					*instructions << "POP ecx" << endl;
				}
			} else if (child_chain_id == 101) { //-
				computeExpression(instructions, locals, node->child[node->child_count - 1], lex);
				*instructions << "POP ebx" << endl;
				*instructions << "POP eax" << endl;
				*instructions << "SUB eax, ebx" << endl;
				*instructions << "PUSH eax" << endl;
			} else if (child_chain_id == 102) { //*
				computeExpression(instructions, locals, node->child[node->child_count - 1], lex);
				*instructions << "POP ebx" << endl;
				*instructions << "POP eax" << endl;
				*instructions << "XOR EDX, EDX" << endl;
				*instructions << "MUL ebx" << endl;
				*instructions << "PUSH eax" << endl;
			} if (child_chain_id == 103) { // /
				computeExpression(instructions, locals, node->child[node->child_count - 1], lex);
				*instructions << "POP ebx" << endl;
				*instructions << "POP eax" << endl;
				*instructions << "XOR EDX, EDX" << endl;
				*instructions << "DIV ebx" << endl;
				*instructions << "PUSH eax" << endl;
			}
		} else if (node->symb == GRB::NS('M')) {
			computeExpression(instructions, locals, node->child[1], lex);
		}
	}

	// генерация списка параметров функции для вызова функции
	std::string GenCode::generateInvokeParams(std::stringstream* instructions, std::stringstream* locals, AST::ASTNode* node, Lexer::LEX* lex, int& maxP, int& usedP) {
		IT::Entry* entry = &(lex->IDtable.table[node->child[0]->lexem->idxTI]);
		std::string s;
		if (node->chain_id == 100 || node->chain_id == 101) {
			if (entry->iddatatype == IT::IDDATATYPE::INT && (!entry->isArray)) {
				s = entry->id;
			} else {
				if (entry->idtype == IT::IDTYPE::P) {
					s = std::string(entry->id);
				} else {
					s = "addr " + std::string(entry->id);
				}
			}
		} else {
			if (entry->iddatatype == IT::IDDATATYPE::INT) {
				s = std::to_string(entry->value.vint);
			} else {
				if (usedP >= maxP) {
					*locals << "LOCAL P" << usedP << "[256]:BYTE" << endl;
				}
				*instructions << "PUSH ecx" << endl;
				*instructions << "invoke lstrcpy, addr P" << usedP << ", offset " << getConstName(node->child[0], lex) << endl;
				*instructions << "POP ecx" << endl;
				s = "addr P" + std::to_string(usedP++);
			}
		}
		if (node->chain_id == 101 || node->chain_id == 103) {
			s = s + ",";
			s = s + generateInvokeParams(instructions, locals, node->child[2], lex, maxP, usedP);
		}
		return s;
	}

	// генерация инструкций тела функции, за исключением инструкций цикла
	void GenCode::generateInstructions(std::stringstream* instructions, std::stringstream* locals, AST::ASTNode* node, Lexer::LEX* lex) {
		//LOCAL testarray [4]:DWORD
		if (node->chain_id == 100 || node->chain_id == 101) {
			IT::Entry* entry = &(lex->IDtable.table[node->child[2]->lexem->idxTI]);
			*locals << "LOCAL " << entry->id << " [" << entry->value.vint << "]:SDWORD" << endl;
			if (node->chain_id == 101) {
				generateInstructions(instructions, locals, node->child[7], lex);
			}
		}
		if (node->chain_id == 102 || node->chain_id == 103 || node->chain_id == 116 || node->chain_id == 117) {
			IT::Entry* entry = &(lex->IDtable.table[node->child[2]->lexem->idxTI]);
			if (entry->iddatatype == IT::IDDATATYPE::INT) {
				*locals << "LOCAL " << entry->id << ":SDWORD" << endl;
			} else {
				*locals << "LOCAL " << entry->id << " [256]:BYTE" << endl;
			}
			if (node->chain_id == 116 || node->chain_id == 117) {
				// вычисляем выражение 'E'
				// результат вычисления должен находиться на вершине стека
				computeExpression(instructions, locals, node->child[4], lex);
				// копируем результать в объявленную выше переменную
				if (entry->iddatatype == IT::IDDATATYPE::INT) {
					*instructions << "POP eax" << endl;
					*instructions << "MOV " << entry->id << ", eax" << endl;
				} else {
					*instructions << "PUSH ecx" << endl;
					if (entry->idtype == IT::IDTYPE::P) {
						*instructions << "invoke lstrcpy, " << entry->id << ", addr S1" << endl;
					} else {
						*instructions << "invoke lstrcpy, addr " << entry->id << ", addr S1" << endl;
					}
					*instructions << "POP ecx" << endl;
				}
			}
			if (node->chain_id == 103) {
				generateInstructions(instructions, locals, node->child[4], lex);
			}
			if (node->chain_id == 117) {
				generateInstructions(instructions, locals, node->child[6], lex);
			}
		}
		if (node->chain_id == 106 || node->chain_id == 107) {
			IT::Entry* entry = &(lex->IDtable.table[node->child[0]->lexem->idxTI]);
			computeExpression(instructions, locals, node->child[2], lex);
			if (entry->iddatatype == IT::IDDATATYPE::INT) {
				*instructions << "POP eax" << endl;
				*instructions << "MOV " << entry->id << ", eax" << endl;
			} else {
				*instructions << "PUSH ecx" << endl;
				if (entry->idtype == IT::IDTYPE::P) {
					*instructions << "invoke lstrcpy, " << entry->id << ", addr S1" << endl;
				} else {
					*instructions << "invoke lstrcpy, addr " << entry->id << ", addr S1" << endl;
				}
				*instructions << "POP ecx" << endl;
			}
			if (node->chain_id == 107) {
				generateInstructions(instructions, locals, node->child[4], lex);
			}
		}
		if (node->chain_id == 108 || node->chain_id == 109 || node->chain_id == 110 || node->chain_id == 111) {
			IT::Entry* entry = &(lex->IDtable.table[node->child[0]->lexem->idxTI]);
			IT::Entry* index = &(lex->IDtable.table[node->child[2]->lexem->idxTI]);
			computeExpression(instructions, locals, node->child[5], lex);
			if (node->chain_id == 108 || node->chain_id == 109) {
				*instructions << "POP eax" << endl;
				*instructions << "MOV " << entry->id << "[" << index->value.vint << "*4], eax" << endl;
			} else {
				*instructions << "XOR edx, edx" << endl;
				*instructions << "MOV eax, " << index->id << endl;
				*instructions << "PUSH ebx" << endl;
				*instructions << "MOV ebx, 4" << endl;
				*instructions << "MUL ebx"<< endl;
				*instructions << "POP ebx" << endl;
				*instructions << "MOV edx, eax" << endl;
				*instructions << "POP eax" << endl;
				*instructions << "MOV " << entry->id << "[edx], eax" << endl;
			}
			if (node->chain_id == 109 || node->chain_id == 111) {
				generateInstructions(instructions, locals, node->child[7], lex);
			}
		}
		if (node->chain_id == 114 || node->chain_id == 115) {
			IT::Entry* entry = &(lex->IDtable.table[node->child[1]->lexem->idxTI]);
			if (entry->iddatatype == IT::IDDATATYPE::INT) {
				*instructions << "push ecx" << endl;
				*instructions << "INVOKE GetStdHandle, STD_OUTPUT_HANDLE" << endl;
				*instructions << "mov consoleOutHandle1, eax" << endl;
				*instructions << "pushad" << endl;
				*instructions << "mov ebx, " << entry->id << endl;
				*instructions << "INVOKE dwtoa, ebx, addr buffer1" << endl;
				*instructions << "INVOKE lstrlen, addr buffer1" << endl;
				*instructions << "mov edx, offset buffer1" << endl;
				*instructions << "INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0" << endl;
				*instructions << "INVOKE lstrlen, addr newline1" << endl;
				*instructions << "mov edx, offset newline1" << endl;
				*instructions << "INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0" << endl;
				*instructions << "popad" << endl;
				*instructions << "pop ecx" << endl;
			} else {
				*instructions << "push ecx" << endl;
				*instructions << "INVOKE GetStdHandle, STD_OUTPUT_HANDLE" << endl;
				*instructions << "mov consoleOutHandle1, eax" << endl;
				*instructions << "pushad" << endl;
				if (entry->idtype == IT::IDTYPE::P) {
					*instructions << "INVOKE lstrlen, " << entry->id << endl;
					*instructions << "INVOKE WriteConsoleA, consoleOutHandle1, " << entry->id << ", eax, offset bytesWritten1, 0" << endl;
				} else {
					*instructions << "INVOKE lstrlen, addr " << entry->id << endl;
					*instructions << "INVOKE WriteConsoleA, consoleOutHandle1, addr " << entry->id << ", eax, offset bytesWritten1, 0" << endl;
				}
				*instructions << "INVOKE lstrlen, addr newline1" << endl;
				*instructions << "mov edx, offset newline1" << endl;
				*instructions << "INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0" << endl;
				*instructions << "popad" << endl;
				*instructions << "pop ecx" << endl;
			}
			if (node->chain_id == 115) {
				generateInstructions(instructions, locals, node->child[3], lex);
			}
		}
		if (node->chain_id == 118 || node->chain_id == 119 || node->chain_id == 120 || node->chain_id == 121) {
			IT::Entry* entry = &(lex->IDtable.table[node->child[2]->lexem->idxTI]);
			if (node->chain_id == 118 || node->chain_id == 119) {
				*instructions << "mov ecx, " << entry->value.vint << endl;
			} else {
				*instructions << "mov ecx, " << entry->id << endl;
			}
			*instructions << ".while ecx > 0" << endl;
			generateLoopInstructions(instructions, locals, node->child[5], lex);
			*instructions << "dec ecx" << endl;
			*instructions << ".endw" << endl;
			if (node->chain_id == 119 || node->chain_id == 121) {
				generateInstructions(instructions, locals, node->child[8], lex);
			}
		}
	}

	// генерация инструкций цикла
	void GenCode::generateLoopInstructions(std::stringstream* instructions, std::stringstream* locals, AST::ASTNode* node, Lexer::LEX* lex) {
		if (node->chain_id == 100 || node->chain_id == 101) {
			IT::Entry* entry = &(lex->IDtable.table[node->child[0]->lexem->idxTI]);
			computeExpression(instructions, locals, node->child[2], lex);
			if (entry->iddatatype == IT::IDDATATYPE::INT) {
				*instructions << "POP eax" << endl;
				*instructions << "MOV " << entry->id << ", eax" << endl;
			} else {
				*instructions << "PUSH ecx" << endl;
				if (entry->idtype == IT::IDTYPE::P) {
					*instructions << "invoke lstrcpy, " << entry->id << ", addr S1" << endl;
				} else {
					*instructions << "invoke lstrcpy, addr " << entry->id << ", addr S1" << endl;
				}
				*instructions << "POP ecx" << endl;
			}
			if (node->chain_id == 101) {
				generateLoopInstructions(instructions, locals, node->child[4], lex);
			}
		}
		if (node->chain_id == 102 || node->chain_id == 103 || node->chain_id == 104 || node->chain_id == 105) {
			IT::Entry* entry = &(lex->IDtable.table[node->child[0]->lexem->idxTI]);
			IT::Entry* index = &(lex->IDtable.table[node->child[2]->lexem->idxTI]);
			computeExpression(instructions, locals, node->child[5], lex);
			if (node->chain_id == 102 || node->chain_id == 103) {
				*instructions << "POP eax" << endl;
				*instructions << "MOV " << entry->id << "[" << index->value.vint << "*4], eax" << endl;
			} else {
				*instructions << "XOR edx, edx" << endl;
				*instructions << "MOV eax, " << index->id << endl;
				*instructions << "PUSH ebx" << endl;
				*instructions << "MOV ebx, 4" << endl;
				*instructions << "MUL ebx"<< endl;
				*instructions << "POP ebx" << endl;
				*instructions << "MOV edx, eax" << endl;
				*instructions << "POP eax" << endl;
				*instructions << "MOV " << entry->id << "[edx], eax" << endl;
			}
			if (node->chain_id == 103 || node->chain_id == 105) {
				generateLoopInstructions(instructions, locals, node->child[7], lex);
			}
		}
		if (node->chain_id == 106 || node->chain_id == 107) {
			IT::Entry* entry = &(lex->IDtable.table[node->child[1]->lexem->idxTI]);
			if (entry->iddatatype == IT::IDDATATYPE::INT) {
				*instructions << "push ecx" << endl;
				*instructions << "INVOKE GetStdHandle, STD_OUTPUT_HANDLE" << endl;
				*instructions << "mov consoleOutHandle1, eax" << endl;
				*instructions << "pushad" << endl;
				*instructions << "mov ebx, " << entry->id << endl;
				*instructions << "INVOKE dwtoa, ebx, addr buffer1" << endl;
				*instructions << "INVOKE lstrlen, addr buffer1" << endl;
				*instructions << "mov edx, offset buffer1" << endl;
				*instructions << "INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0" << endl;
				*instructions << "INVOKE lstrlen, addr newline1" << endl;
				*instructions << "mov edx, offset newline1" << endl;
				*instructions << "INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0" << endl;
				*instructions << "popad" << endl;
				*instructions << "pop ecx" << endl;
			} else {
				*instructions << "push ecx" << endl;
				*instructions << "INVOKE GetStdHandle, STD_OUTPUT_HANDLE" << endl;
				*instructions << "mov consoleOutHandle1, eax" << endl;
				*instructions << "pushad" << endl;
				if (entry->idtype == IT::IDTYPE::P) {
					*instructions << "INVOKE lstrlen, " << entry->id << endl;
					*instructions << "INVOKE WriteConsoleA, consoleOutHandle1, " << entry->id << ", eax, offset bytesWritten1, 0" << endl;
				} else {
					*instructions << "INVOKE lstrlen, addr " << entry->id << endl;
					*instructions << "INVOKE WriteConsoleA, consoleOutHandle1, addr " << entry->id << ", eax, offset bytesWritten1, 0" << endl;
				}
				*instructions << "INVOKE lstrlen, addr newline1" << endl;
				*instructions << "mov edx, offset newline1" << endl;
				*instructions << "INVOKE WriteConsoleA, consoleOutHandle1, edx, eax, offset bytesWritten1, 0" << endl;
				*instructions << "popad" << endl;
				*instructions << "pop ecx" << endl;
			}
			if (node->chain_id == 107) {
				generateLoopInstructions(instructions, locals, node->child[3], lex);
			}
		}
	}

	// получить имя константы (имя - уникальное)
	std::string GenCode::getConstName(AST::ASTNode* node, Lexer::LEX* lex) {
		return std::string(lex->IDtable.table[node->lexem->idxTI].vis) + std::string("_") + std::to_string(node->lexem->idxTI);
	}

	// пишем константы во время обхода дерева
	void GenCode::generateConstants(std::ofstream* stream, AST::ASTNode* node, Lexer::LEX* lex) {
		if (node->lexem->lexema[0] == LEX_LITERAL && lex->IDtable.table[node->lexem->idxTI].iddatatype == IT::IDDATATYPE::STR) {
			*stream << getConstName(node, lex) << " BYTE \"" << lex->IDtable.table[node->lexem->idxTI].value.vstr.str << "\", 0" << endl;
		}
		for (int i = 0; i < node->child_count; i++) {
			generateConstants(stream, node->child[i], lex);
		}
	}

	// генерация тела функции
	void GenCode::generateFunction(std::ofstream* stream, AST::ASTNode* node, Lexer::LEX* lex) {
		stringstream instructions;
		stringstream locals;

		// имя функции и ее параметры
		if (node->chain_id == 100) {
			*stream << lex->IDtable.table[node->child[2]->lexem->idxTI].id << " PROC ";
			generateParams(&instructions, &locals, node->child[4], lex);
			*stream << instructions.str() << endl;
			instructions.clear();
			instructions.str("");
			*stream << endl;
		} else {
			*stream << "main PROC " << endl;
		}

		// локальные переменные
		*stream << "LOCAL S1 [256]:BYTE" << endl; // для конкатенации строк
		*stream << "LOCAL S2 [256]:BYTE" << endl;

		// генерация инструкций
		if (node->chain_id == 100) {
			generateInstructions(&instructions, &locals, node->child[7], lex);
		} else {
			generateInstructions(&instructions, &locals, node->child[2], lex);
		}

		*stream << locals.str() << endl; // пишем все локальные переменные
		locals.clear();
		locals.str("");

		*stream << "MOV S1[0], 0" << endl;
		*stream << "MOV S2[0], 0" << endl;

		*stream << instructions.str() << endl; // пишем все инструкции
		instructions.clear();
		instructions.str("");

		// пишем ret E;
		AST::ASTNode* retNode;
		if (node->chain_id == 100) {
			retNode = node->child[9];
		} else {
			retNode = node->child[4];
		}
		computeExpression(&instructions, &locals, retNode, lex);
		*stream << instructions.str() << endl;
		instructions.clear();
		instructions.str("");
		if (retNode->isInteger) {
			*stream << "POP eax" << endl;
		} else {
			*stream << "invoke lstrcpy, addr retStr1, addr S1" << endl;
			*stream << "LEA eax, retStr1" << endl;
		}
		*stream << "RET" << endl;

		// конец функции
		if (node->chain_id == 100) {
			*stream << lex->IDtable.table[node->child[2]->lexem->idxTI].id << " ENDP" << endl;
			generateFunction(stream, node->child[13], lex);
		} else {
			*stream << "main ENDP" << endl;
		}

		if (node->parent == NULL) {
			*stream << "end main" << endl;
		}
	}

	// главный метод генерации, он пишет заголовок, затем строковые константы и функции (как пользовательские, так и функцию main)
	void GenCode::generate(std::ofstream* stream, AST::ASTNode * node, Lexer::LEX * lex)
	{
		createdTempP = 0;
		*stream << ".586" << endl;
		*stream << ".model flat, stdcall" << endl;
		*stream << "include kernel32.inc" << endl;
		*stream << "include user32.inc" << endl;
		*stream << "include masm32.inc" << endl;
		*stream << "includelib kernel32.lib" << endl;
		*stream << "includelib user32.lib" << endl;
		*stream << "includelib masm32.lib" << endl;
		*stream << "includelib mylibrary.lib" << endl;
		*stream << "ExitProcess PROTO :DWORD" << endl << "STD_OUTPUT_HANDLE EQU -11" << endl;
		*stream << "GetStdHandle PROTO, nStdHandle: DWORD" << endl;
		*stream << "sum PROTO :DWORD, :SDWORD" << endl;
		*stream << "max PROTO :DWORD, :SDWORD" << endl;
		*stream << ".stack 4096" << endl;
		*stream << ".const" << endl;
		*stream << ".data" << endl ;
		*stream << "consoleOutHandle1 dd ?" << endl;
		*stream << "bytesWritten1 dd ?" << endl;
		*stream << "lmessage1 dd ?" << endl;
		*stream << "newline1 BYTE 13, 10, 0" << endl;
		*stream << "buffer1 BYTE 128 dup(?)" << endl;
		*stream << "retStr1 BYTE 256 dup(?)" << endl;
		generateConstants(stream, node, lex);
		*stream << ".code" << endl;
		generateFunction(stream, node, lex);
	}
}
