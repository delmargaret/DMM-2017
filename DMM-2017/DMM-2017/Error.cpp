#include "stdafx.h"
#include "Error.h"
namespace Error
{
	//����� ������: 0 - 90 ��������� ������
	//             100 - 109 ������ ����������
	//             110 - 119 ������ �������� � ����� ������
	ERROR errors[ERROR_MAX_ENTRY] = // ������� ������
	{
		ERROR_ENTRY(0, "������ �������: ������������ ��� ������"), // ��� ������ ��� �������� 0 - ERROR_MAX_ENTRY
		ERROR_ENTRY(1, "������ �������: ��������� ����"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "������ �������� ����: �������� -in ������ ���� �����"),
		ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104, "������ �������� ����: ��������� ����� �������� ���������"),
		ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110, "������ �������� ����: ������ ��� �������� ����� � �������� ����� (-in)"),
		ERROR_ENTRY(111, "������ �������� ����: ������������ ������ � �������� ����� (-in)"),
		ERROR_ENTRY(112, "������ �������� ����: ������ ��� �������� ����� ��������� (-log)"),
		ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115), 
		ERROR_ENTRY(116, "������ �������� ����: �������� ���������� ������ �������� �����"),
		ERROR_ENTRY(117, "������ �������� ����: ��������� ���������� ���������� �������"),
		ERROR_ENTRY(118, "������ �������� ����: ��������� ������������ ������ ������"),
		ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF10(200), ERROR_ENTRY_NODEF(210), ERROR_ENTRY_NODEF(211), ERROR_ENTRY_NODEF(212),
		ERROR_ENTRY(213, "����������� ������: ����������� ������� �� ������� �����"), 
		ERROR_ENTRY(214, "����������� ������: ����������� �������������"), 
		ERROR_ENTRY_NODEF(215), ERROR_ENTRY_NODEF(216), ERROR_ENTRY_NODEF(217), ERROR_ENTRY_NODEF(218),
		ERROR_ENTRY(219, "����������� ������: �������� �������� �������������� ��������"),
		ERROR_ENTRY(220, "����������� ������: ��������� ������������ ����� ���������� ��������"),
		ERROR_ENTRY_NODEF(221), ERROR_ENTRY_NODEF(222), ERROR_ENTRY_NODEF(223), ERROR_ENTRY_NODEF(224), ERROR_ENTRY_NODEF(225),
		ERROR_ENTRY_NODEF(226), ERROR_ENTRY_NODEF(227), ERROR_ENTRY_NODEF(228), ERROR_ENTRY_NODEF(229),
		ERROR_ENTRY_NODEF10(230), ERROR_ENTRY_NODEF10(240), ERROR_ENTRY_NODEF10(250),
		ERROR_ENTRY_NODEF10(260), ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),
		ERROR_ENTRY(300, "�������������� ������: �������� ��������� ���������"),
		ERROR_ENTRY(301, "�������������� ������: ��������� ��������"), 
		ERROR_ENTRY_NODEF(302), ERROR_ENTRY_NODEF(303), ERROR_ENTRY_NODEF(304),ERROR_ENTRY_NODEF(305),
		ERROR_ENTRY(306, "�������������� ������: ����������� ������� ������ ���� �������������"), 
		ERROR_ENTRY_NODEF(307),
		ERROR_ENTRY(308, "�������������� ������: �������� ��� ��������� ��� ������"), 
		ERROR_ENTRY(309, "�������������� ������: ���������� ���������� ����� ����� ���� ������ �������������"), 
		ERROR_ENTRY_NODEF(310), ERROR_ENTRY_NODEF(311), ERROR_ENTRY_NODEF(312), ERROR_ENTRY_NODEF(313), ERROR_ENTRY_NODEF(314), 
		ERROR_ENTRY(315, "�������������� ������: ��������� � ������� ��� �������"),
		ERROR_ENTRY_NODEF(316), ERROR_ENTRY_NODEF(317), ERROR_ENTRY_NODEF(318), ERROR_ENTRY_NODEF(319), ERROR_ENTRY_NODEF(320), 
		ERROR_ENTRY_NODEF(321), ERROR_ENTRY_NODEF(322), ERROR_ENTRY_NODEF(323), ERROR_ENTRY_NODEF(324),
		ERROR_ENTRY(325, "�������������� ������: ����������� ������� �� ������ ���� ������ 1000"),
		ERROR_ENTRY_NODEF(326), ERROR_ENTRY_NODEF(327), ERROR_ENTRY_NODEF(328), ERROR_ENTRY_NODEF(329),
		ERROR_ENTRY_NODEF10(330), ERROR_ENTRY_NODEF10(340), ERROR_ENTRY_NODEF10(350), ERROR_ENTRY_NODEF10(360), ERROR_ENTRY_NODEF10(370),
		ERROR_ENTRY_NODEF10(380), ERROR_ENTRY_NODEF10(390), 
		ERROR_ENTRY_NODEF(400), ERROR_ENTRY_NODEF(401),
		ERROR_ENTRY(402, "������������� ������: ������ � ���������"), 
		ERROR_ENTRY(403, "������������� ������: ������ � ���������� �������"), 
		ERROR_ENTRY(404, "������������� ������: ������ � ���������� ���������� �������"), 
		ERROR_ENTRY(405, "������������� ������: ������ � �������������� ���������"), 
		ERROR_ENTRY(406, "������������� ������: ������� �� ����"), 
		ERROR_ENTRY(407, "������������� ������: ��������� ����������� ��������������"), 
		ERROR_ENTRY_NODEF(408),
		ERROR_ENTRY_NODEF(409),
		ERROR_ENTRY(410, "������������� ������: ������� �� ����� ���� ���������� ������"),
		ERROR_ENTRY(411, "������������� ������: �������� ������������� ����� �������"),
		ERROR_ENTRY(412, "������������� ������: ������� �� �������"),
		ERROR_ENTRY(413, "������������� ������: �������� ����� ��������"),
		ERROR_ENTRY(414, "������������� ������: ���������������� ��� �������� ��� ��������� ������"),
		ERROR_ENTRY_NODEF(415),
		ERROR_ENTRY(416, "������������� ������: ������������� ���� � ���������"),
		ERROR_ENTRY(417, "������������� ������: ��������� ������������ ���������� ���������� �������"),
		ERROR_ENTRY(418, "������������� ������: ������ �� ����� ���� ���������� �������"),
		ERROR_ENTRY(419, "������������� ������: ����������� ��� ��������� ������� �� ��������� � ��������"),
		ERROR_ENTRY(420, "������������� ������: ���������� �������� ������� ��� ��������� �������"),
		ERROR_ENTRY(421, "������������� ������: ��������� ����� ���������� �������"),
		ERROR_ENTRY(422, "������������� ������: ������������� ����� ���������� �������"),
		ERROR_ENTRY(423, "������������� ������: ������������ ������������� ���� �������"),
		ERROR_ENTRY(424, "������������� ������: ����� �� ������� �������"),
		ERROR_ENTRY_NODEF(425), ERROR_ENTRY_NODEF(426), ERROR_ENTRY_NODEF(427), ERROR_ENTRY_NODEF(428), ERROR_ENTRY_NODEF(429),
		ERROR_ENTRY_NODEF10(430), ERROR_ENTRY_NODEF10(440), ERROR_ENTRY_NODEF10(450), ERROR_ENTRY_NODEF10(460), ERROR_ENTRY_NODEF10(470),
		ERROR_ENTRY_NODEF10(480), ERROR_ENTRY_NODEF10(490), ERROR_ENTRY_NODEF100(500), ERROR_ENTRY_NODEF100(600), ERROR_ENTRY_NODEF100(700), 
		ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};
	void printError(int id) {
		std::cout << errors[id].id << "," << errors[id].message << " ";
	}
	ERROR geterror(int id)
	{
		ERROR e;
		if (id > 0 && id < ERROR_MAX_ENTRY)
		{
			e = errors[id];
		}
		else
		{
			e = errors[0];
		}
		return e;
	}

	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		ERROR e;
		if (id > 0 && id < ERROR_MAX_ENTRY)
		{
			e = errors[id];
			e.inext.line = line;
			e.inext.col = col;
		}
		else
		{
			e = errors[0];
			e.inext.line = line;
			e.inext.col = col;
		}
		return e;
	}
}