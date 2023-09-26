#include<iostream>
using std::cin;
using std::cout;
using std::endl;

// returns true if and only if the specified huge integer is zero
bool isZero(int hugeInt[], int size);

// returns true if and only if hugeInt1 < hugeInt2
bool less(int hugeInt1[], int hugeInt2[], int size1, int size2);

// --hugeInt
void decrement(int hugeInt[], int size);

// minuend -= subtrahend provided that minuend > subtrahend
void subtraction(int minuend[], int subtrahend[], int &minuendSize, int subtrahendSize);

// multiplicand *= multiplier
void multiplication(int multiplicand[], int multiplier[], int &multiplicandSize, int multiplierSize);

// quotient = dividend / divisor; remainder = dividend % divisor
void division(int dividend[], int divisor[], int quotient[], int remainder[],
	int dividendSize, int divisorSize, int &quotientSize, int &remainderSize);

// hugeInt /= 10, or equivalently, shifts right by one position
void divideBy10(int hugeInt[], int &size);

const int arraySize = 200;

int main()
{
	int t, a, b;
	while (cin >> t >> a >> b)
	{
		cout << "(" << t << "^" << a << "-1)/(" << t << "^" << b << "-1) ";

		if (t == 1)
			cout << "is not an integer with less than 100 digits.\n";
		else if (a == b)
			cout << "1\n";
		else if (a < b)
			cout << "is not an integer with less than 100 digits.\n";
		else
		{
			int dividend[arraySize] = {};
			int divisor[arraySize] = {};
			int quotient[arraySize] = {};
			int remainder[arraySize] = {};

			int dividendSize = 1;
			int divisorSize = 1;
			int quotientSize = 1;
			int remainderSize = 1;

			// put all digits of t into base
			int base[arraySize] = {};
			int baseSize = 0;
			for (int i = t; i > 0; i /= 10) {
				base[baseSize++] = i % 10;
			}

			// dividend = pow( t, a )
			dividend[0] = 1;
			for (int i = 0; i < a; ++i)
			{
				multiplication(dividend, base, dividendSize, baseSize);
				if (dividendSize > arraySize)
					break;
			}
			if (dividendSize > arraySize)
				cout << "is not an integer with less than 100 digits.\n";
			else
			{
				// divisor = pow( t, b )
				divisor[0] = 1;
				for (int i = 0; i < b; ++i)
					multiplication(divisor, base, divisorSize, baseSize);

				decrement(dividend, dividendSize); // --dividend
				decrement(divisor, divisorSize);   // --divisor

				division(dividend, divisor, quotient, remainder,
					dividendSize, divisorSize, quotientSize, remainderSize);

				// quotient is an integer with less than 100 digits
				if (remainderSize < 100 && isZero(dividend, dividendSize))
					for (int i = remainderSize; i >= 0; i--)
						cout << quotient[i];
				else
					cout << "is not an integer with less than 100 digits.";
				cout << endl;
			}
		}
	}
}

// returns true if and only if the specified huge integer is zero
bool isZero(int hugeInt[], int size)
{
	for (int i = 0; i < size; i++)
		if (hugeInt[i] != 0)
			return false;
	return true;
}

// returns true if and only if hugeInt1 < hugeInt2
bool less(int hugeInt1[], int hugeInt2[], int size1, int size2)
{
	if (size1 < size2) return 1;
	else if (size1 > size2)return 0;
	else if (size1 == size2) {
		for (int i = size1 - 1; i >= 0; i--) {
			if (hugeInt1[i] < hugeInt2[i]) { return 1; }
			else if (hugeInt1[i] > hugeInt2[i]) { return 0; }
			if (i == 0)return 0;
		}
	}
}

// --hugeInt
void decrement(int hugeInt[], int size)
{
	--hugeInt[0];
	for (int i = 0; i < size; i++) {
		if (hugeInt[i] < 0) {
			hugeInt[i + 1]--;
			hugeInt[i] = 9;
		}
	}
}

// minuend -= subtrahend provided that minuend > subtrahend
void subtraction(int minuend[], int subtrahend[], int &minuendSize, int subtrahendSize)
{
	for (int i = 0; i < subtrahendSize; i++)
		minuend[i] = minuend[i] - subtrahend[i];

	for (int i = 0; i < minuendSize; i++)
		if (minuend[i] < 0)
		{
			minuend[i] += 10;
			--minuend[i + 1];
		}

	for (int i = minuendSize - 1; i >= 0; i--) {
		if (minuend[i] == 0)
			minuendSize--;
		else if (minuend[i] != 0)break;
	}

}

// multiplicand *= multiplier
void multiplication(int multiplicand[], int multiplier[], int &multiplicandSize, int multiplierSize)
{
	multiplicandSize += multiplierSize;
	if (multiplicandSize > 200)return;
	int product[200], a[200];
	for (int i = 0; i < multiplicandSize; i++)
	{
		product[i] = multiplicand[i];
		a[i] = multiplicand[i];
	}
	for (int i = 0; i < multiplierSize; i++) {
		for (int j = 0; j < multiplicandSize; j++)
		{
			multiplicand[j + i] += a[j] * multiplier[i];
		}
	}
	for (int i = 0; i < multiplicandSize; i++)
	{
		multiplicand[i] -= product[i];
	}
	for (int i = 0; i < multiplicandSize; i++)
		while (multiplicand[i] > 9)
		{
			multiplicand[i + 1] += multiplicand[i] / 10;
			multiplicand[i] = multiplicand[i] % 10;
		}
	for (int i = multiplicandSize - 1; i >= 0; i--) {
		if (multiplicand[i] != 0)break;
		else multiplicandSize--;
	}
}

// quotient = dividend / divisor; remainder = dividend % divisor
void division(int dividend[], int divisor[], int quotient[], int remainder[],
	int dividendSize, int divisorSize, int &quotientSize, int &remainderSize)
{
	int buffer[200] = {}, bufferSize = dividendSize;
	if (dividendSize == 1 && dividend[0] == 0)
	{
		quotientSize = 1;
		quotient[0] = 0;

		remainderSize = 1;
		remainder[0] = 0;

		return;
	}
	if (less(dividend, divisor, dividendSize, divisorSize)) {
		quotientSize = 1;
		quotient[0] = 0;
	}
	else {
		int n = dividendSize - divisorSize;
		quotientSize = dividendSize - divisorSize;
		for (int i = divisorSize - 1; i >= 0; i--)
		{
			buffer[n + i] = divisor[i];
		}
		if (less(dividend, buffer, dividendSize, dividendSize)) {
			divideBy10(buffer, bufferSize);
			buffer[dividendSize - 1] = 0;
			int b = quotientSize - 1;
			while (!less(dividend, buffer, dividendSize, bufferSize)) {
				subtraction(dividend, buffer, dividendSize, bufferSize);
				quotient[b]++;
				if (less(dividend, buffer, dividendSize, bufferSize) && b != 0) { divideBy10(buffer, bufferSize); --b; }
				if (isZero(dividend, dividendSize))return;
			}
		}
		else {
			quotientSize++;
			int b = quotientSize - 1;
			while (!less(dividend, buffer, dividendSize, bufferSize)) {
				subtraction(dividend, buffer, dividendSize, bufferSize);
				quotient[b]++;
				if (less(dividend, buffer, dividendSize, bufferSize) && b != 0) { divideBy10(buffer, bufferSize); --b; }
				if (isZero(dividend, dividendSize))return;
			}
		}
	}
}

// hugeInt /= 10, or equivalently, shifts right by one position
void divideBy10(int hugeInt[], int &size)
{
	if (size == 1)
		hugeInt[0] = 0;
	else
	{
		for (int i = 1; i < size; i++)
			hugeInt[i - 1] = hugeInt[i];

		size--;
		hugeInt[size] = 0;
	}
}