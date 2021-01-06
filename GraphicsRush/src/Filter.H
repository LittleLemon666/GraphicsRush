#pragma once

enum FFilter
{
	ORIGIN,
	UNIFORM,
	CLIPPING,
	BLUEPRINT,
	DRAW,
	MAXFILTER	// nothing, it is used to filterAdvance()
};

class Filter {
public:
	Filter()
	{
		float filterT;
		int n = 5;
		float ave = 0.0;
		for (int i = 0; i < n * n; i++)
		{
			edge_filter1[i] = 0;
		}
		for (int i = 1; i <= 3; i++)
			for (int j = 1; j <= 3; j++)
				edge_filter1[i * n + j] = 1;
		//edge_filter1[n * 2 + 2] = 1;
		for (int i = n / 2; i >= 0; i--)
		{
			filterT = Binomial(n - 1, i);
			edge_filter2[i] = filterT;
			edge_filter2[n - i - 1] = filterT;
			edge_filter2[i * n] = filterT;
			edge_filter2[(n - i - 1) * n] = filterT;
			ave += filterT * (i == n / 2 ? 2 : 4);
		}
		ave--; //(0,0) is repeatly added
		for (int j = 1; j < n; j++)
		{
			for (int i = 1; i < n; i++)
			{
				edge_filter2[j * n + i] = edge_filter2[j * n] * edge_filter2[i];
				ave += edge_filter2[j * n + i];
			}
		}

		for (int j = 0; j < n; j++)
		{
			for (int i = 0; i < n; i++)
			{
				edge_filter1[j * n + i] -= edge_filter2[j * n + i] / ave;
			}
		}
	}
	~Filter()
	{

	}
	float edge_filter1[5 * 5];
	float edge_filter2[5 * 5];

private:
	// Computes n choose s, efficiently
	double Binomial(int n, int s)
	{
		double        res;

		res = 1;
		for (int i = 1; i <= s; i++)
			res = (n - i + 1) * res / i;

		return res;
	}// Binomial
};