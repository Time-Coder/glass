import java.util.ArrayList;

class AnagramSolver
{
	public static ArrayList<String> solve(char[][] data)
	{
		int n_rows = data.length;
		int n_cols = data[0].length;

		String target, reverse_target;

		// line match
		for(int i = 0; i < n_rows; i++)
		{
			for(int length = 1; length <= n_cols; length++)
			{
				for(int j = 0; j <= n_cols-length; j++)
				{
					target = "";
					for(int k = j; k < j+length; k++)
					{
						target += data[i][k];
					}
					reverse_target = new StringBuffer(target).reverse().toString();

					if(dictionary.contains(target))
					{
						result.add(target);
					}
					if(dictionary.contains(reverse_target))
					{
						result.add(reverse_target);
					}
				}
			}
		}

		// column match
		for(int j = 0; j < n_cols; j++)
		{
			for(int length = 1; length <= n_rows; length++)
			{
				for(int i = 0; i <= n_rows-length; i++)
				{
					target = "";
					for(int k = i; k < i+length; k++)
					{
						target += data[k][j];
					}
					reverse_target = new StringBuffer(target).reverse().toString();

					if(dictionary.contains(target))
					{
						result.add(target);
					}
					if(dictionary.contains(reverse_target))
					{
						result.add(reverse_target);
					}
				}
			}
		}

		// to right down match
		for(int i = 0; i < n_rows; i++)
		{
			int i_start = i;
			int j_start = 0;
			int total_length = Math.min(n_rows-i, n_cols);
			for(int length = 1; length <= total_length; length++)
			{
				for(int j = 0; j <= total_length-length; j++)
				{
					target = "";
					for(int k = j; k < j+length; k++)
					{
						target += data[i_start+k][j_start+k];
					}
					reverse_target = new StringBuffer(target).reverse().toString();

					if(dictionary.contains(target))
					{
						result.add(target);
					}
					if(dictionary.contains(reverse_target))
					{
						result.add(reverse_target);
					}
				}
			}
		}
		for(int j = 1; j < n_cols; j++)
		{
			int i_start = 0;
			int j_start = j;
			int total_length = Math.min(n_cols-j, n_rows);
			for(int length = 1; length <= total_length; length++)
			{
				for(int i = 0; i <= total_length-length; i++)
				{
					target = "";
					for(int k = i; k < i+length; k++)
					{
						target += data[i_start+k][j_start+k];
					}
					reverse_target = new StringBuffer(target).reverse().toString();

					if(dictionary.contains(target))
					{
						result.add(target);
					}
					if(dictionary.contains(reverse_target))
					{
						result.add(reverse_target);
					}
				}
			}
		}

		// to left down
		for(int i = 0; i < n_rows; i++)
		{
			int i_start = i;
			int j_start = n_cols-1;
			int total_length = Math.min(n_rows-i, n_cols);
			for(int length = 1; length <= total_length; length++)
			{
				for(int j = 0; j <= total_length-length; j++)
				{
					target = "";
					for(int k = j; k < j+length; k++)
					{
						target += data[i_start+k][j_start-k];
					}
					reverse_target = new StringBuffer(target).reverse().toString();

					if(dictionary.contains(target))
					{
						result.add(target);
					}
					if(dictionary.contains(reverse_target))
					{
						result.add(reverse_target);
					}
				}
			}
		}
		for(int j = n_cols-2; j >= 0; j--)
		{
			int i_start = 0;
			int j_start = j;
			int total_length = Math.min(j+1, n_rows);
			for(int length = 1; length <= total_length; length++)
			{
				for(int i = 0; i <= total_length-length; i++)
				{
					target = "";
					for(int k = i; k < i+length; k++)
					{
						target += data[i_start+k][j_start-k];
					}
					reverse_target = new StringBuffer(target).reverse().toString();

					if(dictionary.contains(target))
					{
						result.add(target);
					}
					if(dictionary.contains(reverse_target))
					{
						result.add(reverse_target);
					}
				}
			}
		}
	}
}