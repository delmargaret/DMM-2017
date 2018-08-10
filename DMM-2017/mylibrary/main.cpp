extern "C" int __declspec(dllexport) __stdcall sum(unsigned int mas, int len);
extern "C" int __declspec(dllexport) __stdcall max(unsigned int mas, int len);

int __declspec(dllexport) __stdcall sum(unsigned int mas, int len) {
	int res = 0;
	for (int i = 0; i < len; i++) {
		res += ((int *)mas)[i];
	}
	return res;
}

int __declspec(dllexport) __stdcall max(unsigned int mas, int len) {
	int res = 0;
	if (len > 0) {
		res = ((int *)mas)[0];
		for (int i = 1; i < len; i++) {
			if (((int *)mas)[i] > res) {
				res = ((int *)mas)[i];
			}
		}
	}
	return res;
}
