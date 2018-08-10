#pragma once

#include "AST.h"
#include <string>
#include <fstream>
#include <sstream>

namespace GENCODE {
struct GenCode {

	struct ComputeNode {
		ComputeNode* left;
		ComputeNode* right;
		AST::ASTNode* val;
		AST::ASTNode* node;
		char operation;
		bool unary;

		ComputeNode(AST::ASTNode* val, ComputeNode* left, ComputeNode* right, AST::ASTNode* node, char operation, bool unary) : val(val), left(left), right(right), node(node), operation(operation), unary(unary) {};
		ComputeNode() : val(NULL), left(NULL), right(NULL), node(NULL), operation(' '), unary(false) {};
	};

	int createdTempP;
	void generate(std::ofstream* stream, AST::ASTNode* node, Lexer::LEX* lex);
	void generateParams(std::stringstream* instructions, std::stringstream* locals, AST::ASTNode* node, Lexer::LEX* lex);
	void generateInstructions(std::stringstream* instructions, std::stringstream* locals, AST::ASTNode* node, Lexer::LEX* lex);
	void generateLoopInstructions(std::stringstream* instructions, std::stringstream* locals, AST::ASTNode* node, Lexer::LEX* lex);
	void computeExpression(std::stringstream* instructions, std::stringstream* locals, AST::ASTNode* node, Lexer::LEX* lex);
	void generateConstants(std::ofstream* stream, AST::ASTNode* node, Lexer::LEX* lex);
	void generateFunction(std::ofstream* stream, AST::ASTNode* node, Lexer::LEX* lex);
	std::string getConstName(AST::ASTNode* node, Lexer::LEX* lex);
	std::string generateInvokeParams(std::stringstream* instructions, std::stringstream* locals, AST::ASTNode* node, Lexer::LEX* lex, int& maxP, int& usedP);
	ComputeNode* GenCode::factor(AST::ASTNode** node, Lexer::LEX* lex);
	ComputeNode* GenCode::term(AST::ASTNode** node, Lexer::LEX* lex);
	ComputeNode* GenCode::expr(AST::ASTNode** node, Lexer::LEX* lex);
	void computeExpression(std::stringstream* instructions, std::stringstream* locals, GenCode::ComputeNode* node, Lexer::LEX* lex);
};
}