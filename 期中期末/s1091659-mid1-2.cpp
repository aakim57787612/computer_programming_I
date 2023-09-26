// Huge integer division
#include<iostream>
using std::cin;
using std::cout;
using std::endl;

#include<cmath>
using std::log10;

// returns true if and only if the specified huge integer is zero
bool isZero(int hugeInt[], int size);

// returns true if and only if hugeInt1 < hugeInt2
bool less(int hugeInt1[], int hugeInt2[], int size1, int size2);

// --hugeInt
void decrement(int hugeInt[], int &size);

// multiplicand *= multiplier
void multiAssign(int multiplicand[], int multiplier[], int &multiplicandSize, int multiplierSize);

// quotient = dividend / divisor; remainder = dividend % divisor
void division(int dividend[], int divisor[], int quotient[], int remainder[],
	int dividendSize, int divisorSize, int &quotientSize, int &remainderSize);

// product = multiplicand * multiplier * ( 10 ^ multiplierPos )
void multiplication(int multiplicand[], int multiplier, int product[],
	int multiplicandSize, int multiplierPos, int &productSize);

// minuend -= subtrahend
void subtraAssign(int minuend[], int subtrahend[], int &minuendSize, int subtrahendSize);

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
		else if (a % b != 0)
			cout << "is not an integer with less than 100 digits.\n";
		else if ((a - b) * static_cast<int>(log10(t)) > 95)
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
			for (int i = t; i > 0; i /= 10)
				base[baseSize++] = i % 10;

			// dividend = pow( t, a )
			dividend[0] = 1;
			for (int i = 0; i < a; ++i)
			{
				multiAssign(dividend, base, dividendSize, baseSize);
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
					multiAssign(divisor, base, divisorSize, baseSize);

				decrement(dividend, dividendSize); // --dividend
				decrement(divisor, divisorSize);   // --divisor

				division(dividend, divisor, quotient, remainder,
					dividendSize, divisorSize, quotientSize, remainderSize);

				// quotient is an integer with less than 100 digits
				if (quotientSize < 100 && isZero(remainder, remainderSize))
					for (int i = quotientSize - 1; i >= 0; i--)
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
	if (size1 < size2)
		return true;
	else if (size1 > size2)
		return false;
	else if (size1 == size2) {
		for (int i = size1 - 1; i >= 0; i--) {
			if (hugeInt1[i] < hugeInt2[i])
				return true;
			else if (hugeInt1[i] > hugeInt2[i])
				return false;
			if (i == 0)
				return false;
		}
	}
}

// --hugeInt
void decrement(int hugeInt[], int &size)
{
	--hugeInt[0];
	for (int i = 0; i < size; i++) {
		if (hugeInt[i] < 0) {
			hugeInt[i] += 10;
			hugeInt[i + 1]--;
		}
	}
	for (int i = size - 1; i >= 0; i--) {
		if (hugeInt[i] == 0)
			size--;
		else if (hugeInt[i] != 0)
			break;
	}
}

// multiplicand *= multiplier
void multiAssign(int multiplicand[], int multiplier[], int &multiplicandSize, int multiplierSize)
{
	multiplicandSize += multiplierSize;

	int product[200] = { };
	for (int j = 0; j < multiplierSize; j++) {
		for (int i = 0; i < multiplicandSize; i++) {
			product[i + j] += multiplicand[i] * multiplier[j];
		}
	}
	for (int i = 0; i < multiplicandSize; i++) {
		while (product[i] > 9) {
			product[i + 1]++;
			product[i] -= 10;
		}//modify
	}
	for (int i = 0; i < multiplicandSize; i++) {
		multiplicand[i] = product[i];
	}

	for (int i = multiplicandSize - 1; i >= 0; i--) {
		if (multiplicand[i] == 0)
			multiplicandSize--;
		else if (multiplicand[i] != 0)
			break;
	}

}

void division(int dividend[], int divisor[], int quotient[], int remainder[],
	int dividendSize, int divisorSize, int &quotientSize, int &remainderSize)
{
	remainderSize = dividendSize;
	if (dividend[0] == 0 && dividendSize == 1)
	{
		quotientSize == 1;
		quotient[0] = 0;
		remainderSize == 1;
		remainder[0] = 0;
		return;
	}

	for (int i = dividendSize - 1; i >= 0; i--) {
		remainder[i] = dividend[i];
	}

	if (less(dividend, divisor, dividendSize, divisorSize)) {
		quotientSize == 1;
		quotient[0] = 0;
		return;
	}

	int  n = dividendSize - divisorSize;
	int buffer[200] = { };
	for (int i = divisorSize - 1; i >= 0; i--) {
		buffer[i] = divisor[i];
	}
	for (int i = divisorSize - 1; i >= 0; i--) {
		buffer[i + n] = buffer[i];
		buffer[i] = 0;
	}

	int bufferSize = divisorSize + n;
	quotientSize = n;
	/*if (!less(dividend, buffer, dividendSize, bufferSize)) {
		quotientSize++;
	}
	else if (less(dividend, buffer, dividendSize, bufferSize)) {
		for (int i = 0; i <= divisorSize; i++) {
			buffer[i] = buffer[i + 1];
			buffer[i + 1] = 0;
			bufferSize--;
		}
	}deleted*/
	for (int i = quotientSize - 1; i >= 0; i--) {
		quotient[i] = 0;
	}

	int a = divisor[divisorSize - 1];
	int j = dividendSize - 1;
	int c = 0;
	for (int i = dividendSize - divisorSize; i >= 0; i--, j--)
	{
		int b = 10 * remainder[j + 1] + remainder[j];
		if (a <= b)
		{
			quotient[i] = b / a;
			multiplication(divisor, quotient[i], buffer,
				divisorSize, i, bufferSize);//modify
			if (!less(dividend, buffer, dividendSize, bufferSize) && c == 0) {
				quotientSize++;
				c++;
			}//added
			while (less(remainder, buffer, remainderSize, bufferSize))
			{
				quotient[i]--;
				multiplication(divisor, quotient[i], buffer,
					divisorSize, i, bufferSize);//modify
			}//added
			subtraAssign(remainder, buffer, remainderSize, bufferSize);//added
		}
		/*else {
			b = 10 * b + dividend[i - 1];
			quotient[i - 1] = b / a;
			multiplication(divisor, quotient[i - 1], buffer,
				divisorSize, i - 1, bufferSize);
		}

		while (less(remainder, buffer, remainderSize, bufferSize))
		{
			quotient[i]--;
			multiplication(divisor, quotient[i], buffer,
				divisorSize, i - 1, bufferSize);
		}
		subtraAssign(remainder, buffer, remainderSize, bufferSize);
		if (isZero(remainder, remainderSize))return; deleted*/
	}
}

void multiplication(int multiplicand[], int multiplier, int product[],
	int multiplicandSize, int multiplierPos, int &productSize)
{
	/*int a = 0;
	for (int i = 100; i >= 0; i--) {
		int multiplier11 = multiplier;
		multiplier11 = multiplier11 / 10;
		if (multiplier11 > 0)a++;
	}
	int multiplierSize = multiplierPos + 1;
	int multiplier1[200] = {};
	multiplier1[multiplierSize - 1] = multiplier;
	productSize = multiplicandSize + multiplierPos + 1 ;deleted*/
	for (int i = 0; i <= productSize; i++) {
		product[i] = { 0 };
	}//added
	for (int j = 0; j <= multiplicandSize; j++) {
		product[j + multiplierPos] += multiplicand[j] * multiplier;
	}//modify
	for (int i = 0; i < productSize; i++) {
		while (product[i] >= 10) {
			product[i] -= 10;
			product[i + 1]++;
		}
	}//modify
	/*for (int i = productSize - 1; i >= 0; i--) {
		if (product[i] == 0)
			productSize--;
		else if (product[i] != 0)
			break;
	}deleted*/
	for (int i = 150; i >= 0; i--) {
		if (product[i] != 0) {
			productSize = i + 1;
			break;
		}
	}//added
}

void subtraAssign(int minuend[], int subtrahend[], int &minuendSize, int subtrahendSize)
{
	for (int i = subtrahendSize - 1; i >= 0; i--) {
		minuend[i] -= subtrahend[i];
	}

	for (int i = 0; i < minuendSize; i++) {
		if (minuend[i] < 0) {
			minuend[i] += 10;
			minuend[i + 1]--;
		}
	}

	for (int i = minuendSize - 1; i >= 0; i--) {
		if (minuend[i] == 0)
			minuendSize--;
		else if (minuend[i] != 0)
			break;
	}

}
