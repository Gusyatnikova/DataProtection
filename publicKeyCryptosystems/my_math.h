#pragma once
#include <vector>
using namespace std;
class my_math
{
public:
	my_math() {}
	~my_math() {}
	//fast calc of pow(x, y) % N
	template <typename T>
	static T modularization(T x, T y, T n) {
		if (0 == y) return 1l;
		T z = modularization(x, y / 2, n);
		if (0 == y % 2) {
			return (z * z) % n;
		}
		else {
			return (x * z * z) % n;
		}
	}
	template <typename T>
	static T mul_modul(T a, T b, T n) {
		int sum = 0;

		for (int i = 0; i < b; i++) {
			sum += a;

			if (sum >= n) {
				sum -= n;
			}
		}

		return sum;
	}
	template <typename T>
	static int generator(T p) {
		vector<T> fact;
		T phi = p - 1, n = phi;
		for (int i = 2; i*i <= n; ++i)
			if (n % i == 0) {
				fact.push_back(i);
				while (n % i == 0)
					n /= i;
			}
		if (n > 1)
			fact.push_back(n);

		for (int res = 2; res <= p; ++res) {
			bool ok = true;
			for (size_t i = 0; i < fact.size() && ok; ++i)
				ok &= modularization(res, phi / fact[i], p) != 1;
			if (ok)  return res;
		}
		return -1;
	}
};


