import java.util.ArrayList;
import java.util.Arrays;

public class Main
{
	public static void main(String[] args)
	{
		KSelector selector = new KSelector();
		int n_max = 1000;
		int m_max = 100;
		ArrayList<Double> n_inputs = new ArrayList<Double>();
		ArrayList<Double> time_elaspe = new ArrayList<Double>();
		for(int i = 1; i <= n_max; i++)
		{
			double t = 0;
			for(int j = 0; j < m_max; j++)
			{
				ArrayList<Integer> array = generate_input(i);
				long start_time = System.nanoTime();
				selector.sort_select2(array, (int)(i/2));
				long end_time = System.nanoTime();
				t += (end_time - start_time);
			}
			time_elaspe.add(1E-6*t/m_max);
			n_inputs.add((double)(i));
		}
		System.out.println("plt.plot(" + n_inputs.toString() + ", " + time_elaspe.toString() + ")");
	}

	public static ArrayList<Integer> generate_input(int n)
	{
		ArrayList<Integer> result = new ArrayList<>();
		for(int i = 0; i < n; i++)
		{
			result.add((int)(Math.random()*100));
		}

		return result;
	}
}