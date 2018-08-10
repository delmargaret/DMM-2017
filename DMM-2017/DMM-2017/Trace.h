#pragma once

using namespace std;

int FST_TRACE_n = -1;
char rbuf[1024], sbuf[1024], lbuf[1024];

#define MFST_TRACE_START mfstout << setw(4) << left << "Шаг " << ": " << setw(20) << left << " Правило" << setw(30) << left << " Входная лента" \
							<< setw(20) << left << " Стек" << endl;

#define MFST_TRACE1 mfstout << setw(4) << left << ++FST_TRACE_n << ": " << setw(20) << left << rule.getCRule(rbuf, nrulechain) \
						 << setw(30) << left << getCLenta(lbuf, lenta_position) << setw(20) << left << getCSt(sbuf) << endl;

#define MFST_TRACE2 mfstout << setw(4) << left << FST_TRACE_n << ": " << setw(20) << left << " " \
						 << setw(30) << left << getCLenta(lbuf, lenta_position) << setw(20) << left << getCSt(sbuf) << endl;

#define MFST_TRACE3 mfstout << setw(4) << left << ++FST_TRACE_n << ": " << setw(20) << left << " " \
						 << setw(30) << left << getCLenta(lbuf, lenta_position) << setw(20) << left << getCSt(sbuf) << endl;

#define MFST_TRACE4(c) mfstout << setw(4) << left << ++FST_TRACE_n << ": " << setw(20) << left << " " << c << endl;

#define MFST_TRACE5(c) mfstout << setw(4) << left << FST_TRACE_n << ": " << setw(20) << left << " " << c << endl;

#define MFST_TRACE6(c, k) mfstout << setw(4) << left << ++FST_TRACE_n << ": " << setw(20) << left << " " << c << k << endl;

#define MFST_TRACE7 mfstout << setw(4) << left << state.lenta_position << ": " << setw(20) << left << rule.getCRule(rbuf, state.nrulechain) << endl;
