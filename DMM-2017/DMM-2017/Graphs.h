#pragma once

#define GRAPH_ARYTHM_PLUS 2,\
	Fst::NODE(11, Fst::RELATION('+',1)),\
	Fst::NODE()

#define GRAPH_ARYTHM_MINUS 2,\
	Fst::NODE(11, Fst::RELATION('-',1)),\
	Fst::NODE()

#define GRAPH_ARYTHM_MULT 2,\
	Fst::NODE(11, Fst::RELATION('*',1)),\
	Fst::NODE()

#define GRAPH_ARYTHM_DIV 2,\
	Fst::NODE(11, Fst::RELATION('/',1)),\
	Fst::NODE()

#define GRAPH_SEMICOLON 2,\
	Fst::NODE(1,Fst::RELATION(';',1)),\
	Fst::NODE()
#define GRAPH_EQUALS 2,\
	Fst::NODE(1,Fst::RELATION('=',1)),\
	Fst::NODE()
#define GRAPH_COMMA 2,\
	Fst::NODE(1,Fst::RELATION(',',1)),\
	Fst::NODE()
#define GRAPH_LEFT_BRACE 2,\
	Fst::NODE(1,Fst::RELATION('{',1)),\
	Fst::NODE()
#define GRAPH_RIGHT_BRACE 2,\
	Fst::NODE(1,Fst::RELATION('}',1)),\
	Fst::NODE()
#define GRAPH_OPEN_PARENTHESIS 2,\
	Fst::NODE(1,Fst::RELATION('(',1)),\
	Fst::NODE()
#define GRAPH_CLOSE_PARENTHESIS 2,\
	Fst::NODE(1,Fst::RELATION(')',1)),\
	Fst::NODE()
#define GRAPH_OPEN_SQUARE 2,\
	Fst::NODE(1,Fst::RELATION('[',1)),\
	Fst::NODE()
#define GRAPH_CLOSE_SQUARE 2,\
	Fst::NODE(1,Fst::RELATION(']',1)),\
	Fst::NODE()

#define GRAPH_POW 4, \
	Fst::NODE(1,Fst::RELATION('P',1)),\
	Fst::NODE(1,Fst::RELATION('O',2)),\
	Fst::NODE(1,Fst::RELATION('W',3)),\
	Fst::NODE()

#define GRAPH_STRLEN 7,\
	Fst::NODE(1,Fst::RELATION('s',1)),\
	Fst::NODE(1,Fst::RELATION('t',2)),\
	Fst::NODE(1,Fst::RELATION('r',3)),\
	Fst::NODE(1,Fst::RELATION('l',4)),\
	Fst::NODE(1,Fst::RELATION('e',5)),\
	Fst::NODE(1,Fst::RELATION('n',6)),\
	Fst::NODE()

#define GRAPH_INT_LITERAL 2, \
	Fst::NODE(10,Fst::RELATION('0',1),Fst::RELATION('1',1),\
				Fst::RELATION('3',1),Fst::RELATION('2',1),\
				Fst::RELATION('5',1),Fst::RELATION('4',1),\
				Fst::RELATION('7',1),Fst::RELATION('6',1),\
				Fst::RELATION('8',1),Fst::RELATION('9',1)),\
	Fst::NODE(10,Fst::RELATION('0',1),Fst::RELATION('1',1),\
				Fst::RELATION('3',1),Fst::RELATION('2',1),\
				Fst::RELATION('5',1),Fst::RELATION('4',1),\
				Fst::RELATION('7',1),Fst::RELATION('6',1),\
				Fst::RELATION('8',1),Fst::RELATION('9',1))

#define GRAPH_STRING_LITERAL 3, \
	Fst::NODE(1, Fst::RELATION('"',1)),\
	Fst::NODE(96, Fst::RELATION(32,1),Fst::RELATION(33,1),Fst::RELATION(34,1),Fst::RELATION(35,1),Fst::RELATION(36,1),Fst::RELATION(37,1),\
	Fst::RELATION(38,1),Fst::RELATION(39,1),Fst::RELATION(40,1),Fst::RELATION(41,1),Fst::RELATION(42,1),Fst::RELATION(43,1),\
	Fst::RELATION(44,1),Fst::RELATION(45,1),Fst::RELATION(46,1),Fst::RELATION(47,1),Fst::RELATION(48,1),Fst::RELATION(49,1),\
Fst::RELATION(50,1),Fst::RELATION(51,1),Fst::RELATION(52,1),Fst::RELATION(53,1),Fst::RELATION(54,1),Fst::RELATION(55,1),\
Fst::RELATION(56,1),Fst::RELATION(57,1),Fst::RELATION(58,1),Fst::RELATION(59,1),Fst::RELATION(60,1),Fst::RELATION(61,1),\
Fst::RELATION(62,1),Fst::RELATION(63,1),Fst::RELATION(64,1),Fst::RELATION(65,1),Fst::RELATION(66,1),Fst::RELATION(67,1),\
Fst::RELATION(68,1),Fst::RELATION(69,1),Fst::RELATION(70,1),Fst::RELATION(71,1),Fst::RELATION(72,1),Fst::RELATION(73,1),\
Fst::RELATION(74,1),Fst::RELATION(75,1),Fst::RELATION(76,1),Fst::RELATION(77,1),Fst::RELATION(78,1),Fst::RELATION(79,1),\
Fst::RELATION(80,1),Fst::RELATION(81,1),Fst::RELATION(82,1),Fst::RELATION(83,1),Fst::RELATION(84,1),Fst::RELATION(85,1),\
Fst::RELATION(86,1),Fst::RELATION(87,1),Fst::RELATION(88,1),Fst::RELATION(89,1),Fst::RELATION(90,1),Fst::RELATION(91,1),\
Fst::RELATION(92,1),Fst::RELATION(93,1),Fst::RELATION(94,1),Fst::RELATION(95,1),Fst::RELATION(96,1),Fst::RELATION(97,1),\
Fst::RELATION(98,1),Fst::RELATION(99,1),Fst::RELATION(100,1),Fst::RELATION(101,1),Fst::RELATION(102,1),Fst::RELATION(103,1),\
Fst::RELATION(104,1),Fst::RELATION(105,1),Fst::RELATION(106,1),Fst::RELATION(107,1),Fst::RELATION(108,1),Fst::RELATION(109,1),\
Fst::RELATION(110,1),Fst::RELATION(111,1),Fst::RELATION(112,1),Fst::RELATION(113,1),Fst::RELATION(114,1),Fst::RELATION(115,1),\
Fst::RELATION(116,1),Fst::RELATION(117,1),Fst::RELATION(118,1),Fst::RELATION(119,1),Fst::RELATION(120,1),Fst::RELATION(121,1),\
Fst::RELATION(122,1),Fst::RELATION(123,1),Fst::RELATION(124,1),Fst::RELATION(125,1),Fst::RELATION(126,1),\
Fst::RELATION('"',2)),\
	Fst::NODE()

#define GRAPH_MAIN 5, \
	Fst::NODE(1,Fst::RELATION('m',1)),\
	Fst::NODE(1,Fst::RELATION('a',2)),\
	Fst::NODE(1,Fst::RELATION('i',3)),\
	Fst::NODE(1,Fst::RELATION('n',4)),\
	Fst::NODE()

#define GRAPH_FOR 4, \
	Fst::NODE(1,Fst::RELATION('f',1)),\
	Fst::NODE(1,Fst::RELATION('o',2)),\
	Fst::NODE(1,Fst::RELATION('r',3)),\
	Fst::NODE()

#define GRAPH_INTEGER 7, \
	Fst::NODE(1,Fst::RELATION('e',1)),\
	Fst::NODE(1,Fst::RELATION('n',2)),\
	Fst::NODE(1,Fst::RELATION('t',3)),\
	Fst::NODE(1,Fst::RELATION('e',4)),\
	Fst::NODE(1,Fst::RELATION('r',5)),\
    Fst::NODE(1,Fst::RELATION('o',6)),\
	Fst::NODE()

#define GRAPH_STRING 7, \
	Fst::NODE(1,Fst::RELATION('c',1)),\
	Fst::NODE(1,Fst::RELATION('u',2)),\
	Fst::NODE(1,Fst::RELATION('e',3)),\
	Fst::NODE(1,Fst::RELATION('r',4)),\
	Fst::NODE(1,Fst::RELATION('d',5)),\
	Fst::NODE(1,Fst::RELATION('a',6)),\
	Fst::NODE()

#define GRAPH_FUNCTION 9, \
	Fst::NODE(1, Fst::RELATION('f', 1)),\
    Fst::NODE(1, Fst::RELATION('u', 2)),\
    Fst::NODE(1, Fst::RELATION('n', 3)),\
    Fst::NODE(1, Fst::RELATION('c', 4)),\
    Fst::NODE(1, Fst::RELATION('t', 5)),\
    Fst::NODE(1, Fst::RELATION('i', 6)),\
    Fst::NODE(1, Fst::RELATION('o', 7)),\
    Fst::NODE(1, Fst::RELATION('n', 8)),\
    Fst::NODE()

#define GRAPH_RETURN 4, \
	Fst::NODE(1,Fst::RELATION('r',1)),\
	Fst::NODE(1,Fst::RELATION('e',2)),\
	Fst::NODE(1,Fst::RELATION('t',3)),\
	Fst::NODE()

#define GRAPH_DECLARE 8, \
    Fst::NODE(1, Fst::RELATION('d', 1)),\
    Fst::NODE(1, Fst::RELATION('e', 2)),\
    Fst::NODE(1, Fst::RELATION('c', 3)),\
    Fst::NODE(1, Fst::RELATION('l', 4)),\
    Fst::NODE(1, Fst::RELATION('a', 5)),\
    Fst::NODE(1, Fst::RELATION('r', 6)),\
    Fst::NODE(1, Fst::RELATION('e', 7)),\
    Fst::NODE()

#define GRAPH_PRINT 6,	\
	Fst::NODE(1, Fst::RELATION('p', 1)),\
    Fst::NODE(1, Fst::RELATION('r', 2)),\
    Fst::NODE(1, Fst::RELATION('i', 3)),\
    Fst::NODE(1, Fst::RELATION('n', 4)),\
    Fst::NODE(1, Fst::RELATION('t', 5)),\
    Fst::NODE()

#define GRAPH_ARRAY 7,	\
	Fst::NODE(1, Fst::RELATION('m', 1)),\
    Fst::NODE(1, Fst::RELATION('a', 2)),\
    Fst::NODE(1, Fst::RELATION('s', 3)),\
    Fst::NODE(1, Fst::RELATION('s', 4)),\
    Fst::NODE(1, Fst::RELATION('i', 5)),\
	Fst::NODE(1, Fst::RELATION('v', 6)),\
    Fst::NODE()

#define GRAPH_ID 2, \
	Fst::NODE(26, Fst::RELATION('a',1),  Fst::RELATION('b',1), \
				 Fst::RELATION('c', 1),  Fst::RELATION('d', 1), \
				 Fst::RELATION('e', 1),  Fst::RELATION('f', 1), \
				 Fst::RELATION('g', 1),  Fst::RELATION('h', 1), \
				 Fst::RELATION('i', 1),  Fst::RELATION('j', 1), \
				 Fst::RELATION('k', 1),  Fst::RELATION('l', 1), \
				 Fst::RELATION('m', 1),  Fst::RELATION('n', 1), \
				 Fst::RELATION('o', 1),  Fst::RELATION('p', 1), \
				 Fst::RELATION('q', 1),  Fst::RELATION('r', 1), \
				 Fst::RELATION('s', 1),  Fst::RELATION('t', 1), \
				 Fst::RELATION('u', 1),  Fst::RELATION('v', 1), \
				 Fst::RELATION('w', 1),  Fst::RELATION('x', 1), \
				 Fst::RELATION('y', 1),  Fst::RELATION('z', 1)), \
	Fst::NODE(26, Fst::RELATION('a',1),  Fst::RELATION('b',1), \
				 Fst::RELATION('c', 1),  Fst::RELATION('d', 1), \
				 Fst::RELATION('e', 1),  Fst::RELATION('f', 1), \
				 Fst::RELATION('g', 1),  Fst::RELATION('h', 1), \
				 Fst::RELATION('i', 1),  Fst::RELATION('j', 1), \
				 Fst::RELATION('k', 1),  Fst::RELATION('l', 1), \
				 Fst::RELATION('m', 1),  Fst::RELATION('n', 1), \
				 Fst::RELATION('o', 1),  Fst::RELATION('p', 1), \
				 Fst::RELATION('q', 1),  Fst::RELATION('r', 1), \
				 Fst::RELATION('s', 1),  Fst::RELATION('t', 1), \
				 Fst::RELATION('u', 1),  Fst::RELATION('v', 1), \
				 Fst::RELATION('w', 1),  Fst::RELATION('x', 1), \
				 Fst::RELATION('y', 1),  Fst::RELATION('z', 1))
