import java.util.ArrayList;

public class KSelector
{
	public static <DataType extends Comparable<? super DataType>> DataType sort_select1(ArrayList<DataType> array, int k)
	{
		situ_merge_sort(array, 0, array.size());
		return array.get(k);
	}

	public static <DataType extends Comparable<? super DataType>> DataType sort_select2(ArrayList<DataType> array, int k)
	{
		situ_merge_sort(array, 0, k+1);
		for(int i = k+1; i < array.size(); i++)
		{
			for(int j = 0; j <= k; j++)
			{
				if(array.get(i).compareTo(array.get(j)) > 0)
				{
					for(int l = k; l >= j+1; l--)
					{
						array.set(l, array.get(l-1));
					}
					array.set(j, array.get(i));
					break;
				}
			}
		}

		return array.get(k);
	}

	public static <DataType extends Comparable<? super DataType>> void bubble_sort(ArrayList<DataType> array, int begin, int end)
	{
		for(int i = end-2; i >= begin; i--)
		{
			for(int j = begin; j <= i; j++)
			{
				if(array.get(j).compareTo(array.get(j+1)) < 0)
				{
					DataType temp = array.get(j);
					array.set(j, array.get(j+1));
					array.set(j+1, temp);
				}
			}
		}
	}

	private static <DataType extends Comparable<? super DataType>> void reverse(ArrayList<DataType> array, int start, int end)
	{
		int i = start, j = end-1;
		DataType temp;
		while(j > i)
		{
			temp = array.get(i);
			array.set(i, array.get(j));
			array.set(j, temp);
			i++;
			j--;
		}
	}

	public static <DataType extends Comparable<? super DataType>> void situ_merge_sort(ArrayList<DataType> array, int start, int end)
	{
		if(end - start <= 1)
		{
			return;
		}
		
		int center = (start + end) / 2;
		situ_merge_sort(array, start, center);
		situ_merge_sort(array, center, end);
		int i = start, j = center;
		
		while(i < center && array.get(i).compareTo(array.get(j)) >= 0)
		{
			i++;
		}
		while(j < end && array.get(j).compareTo(array.get(i)) >= 0)
		{
			j++;
		}
		reverse(array, i, center);
		reverse(array, center, j);
		reverse(array, i, j);
		situ_merge_sort(array, i+j-center, end);
	}
}