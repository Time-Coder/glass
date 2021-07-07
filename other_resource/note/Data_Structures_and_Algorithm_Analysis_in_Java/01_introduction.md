# 引论

# 经典问题
* $k$ 选择问题：$N$ 个数，选择其中的第 $k$ 大者。其有两种直观的实现方法：
    * 递减顺序排序，返回第 $k$ 个位置的数。代码见 [KSelector/sort_select1](Code/k_selection/src.html/KSelector.java.html)
    * 读入前 $k$ 个数到数组，以递减的顺序排序这 $k$ 个数；随后逐个读入剩余的数，如果它小于数组中第 $k$ 个数，则忽略之；否则插入到正确的位置上，这将挤掉数组中的第 $k$ 个数。代码见 [KSelector/sort_select2](Code/k_selection/src.html/KSelector.java.html)
* 字谜游戏

# 数学知识

## 基本公式
* **指数运算**：
    * $x^ax^b = x^{a+b}$
    * $\dfrac{x^a}{x^b} = x^{a-b}$
    * $\left(x^a\right)^b = x^{ab}$
* **对数运算**：
    * $a^{\log_ax} = x$
    * $\log_a(xy) = \log_ax + \log_ay$
    * $\log_a\dfrac{x}{y} = \log_ax-\log_ay$
    * $\log_ax^n = n\log_ax$
    * $\log_ab \cdot \log_ba = 1$
    * $\log_ab = \dfrac{\log_cb}{\log_ca}$
* **级数运算**：
	* 前 $n$ 项和（设 $a \neq 1$）：
		* $\sum\limits_{i=1}^n i = \dfrac{n(n+1)}{2}$
		* $\sum\limits_{i=0}^n a^i = \dfrac{1 - a^{n+1}}{1 - a}$
		* $\sum\limits_{i=1}^n ia^i = \dfrac{a}{(a-1)^2}\left(na^{n+1}-(n+1)a^n+1\right)$
		* $\sum\limits_{i=1}^n i^2 = \dfrac{n(n+1)(2n+1)}{6}$
	* 前 $n$ 项和在正无穷处的等价：
		* $\forall k > -1, \sum\limits_{i=1}^n i^k \sim \dfrac{n^{k+1}}{k+1}$
		* $\sum\limits_{i=1}^n \dfrac{1}{i} \sim \ln n$
	* 级数和（设 $a < 1$）：
		* $\sum\limits_{i=0}^{\infty} a^i = \dfrac{1}{1 - a}$
		* $\sum\limits_{i=1}^{\infty} ia^i = \dfrac{a}{(a-1)^2}$
		* $\sum\limits_{i=1}^{\infty} \dfrac{1}{i^2} = \dfrac{\pi^2}{6}$
		* $\forall k > 1, \sum\limits_{i=1}^{\infty} \dfrac{1}{i^k}$ 为常数
		* $\lim\limits_{n\rightarrow\infty}\left(\sum\limits_{i=1}^n\dfrac{1}{i}-\ln n\right) = \gamma \approx 0.57721566$（欧拉数）
* **模运算**：
	* 设 $a, b, n \in \mathbb{R}$，记 $a = b[n]$ 当且仅当 $\exists k \in \mathbb{Z}$ 满足 $a = b + kn$
	* 设 $a, b, n \in \mathbb{R}$ 且有 $a = b[n]$，则
$$
\begin{aligned}
& \forall c \in \mathbb{R}, a+c = (b+c)[n]\\
& \forall c \in \mathbb{Z}, ac = (bc)[n]
\end{aligned}
$$

## 证明方法
* **第一类数学归纳法**<br/>
例：求证：$\forall n \in \mathbb{N}^*, \sum\limits_{i=1}^n i^2 = \dfrac{n(n+1)(2n+1)}{6}$
> **证明**：$\forall n \in \mathbb{N}^*$，定义布尔值函数 $\mathcal{P}(n) = \left(\sum\limits_{i=1}^n i^2 == \dfrac{n(n+1)(2n+1)}{6}\right)$
> 
> * $\mathcal{P}(1) = \mbox{True}$ 的证明：<br/>
>     由于 $\sum\limits_{i=1}^1 i^2 = 1$ 且 $\dfrac{1\times(1+1)(2\times 1+1)}{6} = 1$<br/>
>     故当 $n = 1$ 时，\sum\limits_{i=1}^n i^2 = \dfrac{n(n+1)(2n+1)}{6}
>     故 $\mathcal{P}(1) = \mbox{True}$
> * 设 $k \in \mathbb{N}^*$，假设 $\mathcal{P}(k) = \mbox{True}$。在假设条件下 $\mathcal{P}(k+1) = \mbox{True}$ 的证明：
>     由于
> $$
> \begin{aligned}
> \sum\limits_{i=1}^{k+1} i^2 & = \sum\limits_{i=1}^{k} i^2 + (k+1)^2 \\
>                             & = \dfrac{k(k+1)(2k+1)}{6} + (k+1)^2 \\
>                             & = \dfrac{(k+1)(k+2)(2k+3)}{6} \\
>                             & = \dfrac{(k+1)(k+2)\left(2(k+1)+1\right)}{6}
> \end{aligned}
> $$
>     故 $\mathcal{P}(k+1) = \mbox{True}$
>
> 由第一类数学归纳法得：$\forall n \in \mathbb{N}^*, \mathcal{P}(n) = \mbox{True}$，<br/>
> 即 $\forall n \in \mathbb{N}^*, \sum\limits_{i=1}^n i^2 = \dfrac{n(n+1)(2n+1)}{6}$<br/>
> 证毕。
* **第二类数学归纳法**<br/>
例：我们递归的定义数列 $(F_n)_{n \in \mathbb{N}}$：
$$
\left\{
\begin{aligned}
& F_0 = 1\\
& F_1 = 1\\
& \forall n \in \mathbb{N}, F_{n+2} = F_{n+1} + F_{n}
\end{aligned}
\right.
$$
求证：$\forall n \in \mathbb{N}^{*}, F_n < \left(\dfrac{5}{3}\right)^n$
> **证明**：$\forall n \in \mathbb{N}^{*}$，定义布尔值函数 $\mathcal{P}(n) = \left(F_n < \left(\dfrac{5}{3}\right)^n\right)$
> 
> * $\mathcal{P}(1) = \mbox{True}$ 的证明：</br>
>     由于 $F_1 = 1$ 且 $\left(\dfrac{5}{3}\right)^1 = \dfrac{5}{3} > 1$<br/>
>     故 $F_1 < \left(\dfrac{5}{3}\right)^1$<br/>
>     故 $\mathcal{P}(1) = \mbox{True}$
> * 设 $k \in \mathbb{N}^*\backslash\{1\}$，假设 $\forall i \in [\![1, k]\!], \mathcal{P}(i) = \mbox{True}$。在假设条件下 $\mathcal{P}(k+1) = \mbox{True}$ 的证明：<br/>
>     根据假设，$\mathcal{P}(k) = \mbox{True}, \mathcal{P}(k-1) = \mbox{True}$，即：
> $$
> \left\{
> \begin{aligned}
> & F_k < \left(\dfrac{5}{3}\right)^k\\
> & F_{k-1} < \left(\dfrac{5}{3}\right)^{k-1}
> \end{aligned}
> \right.
> $$
>     故
> $$
> \begin{aligned}
> F_{k+1} & = F_k + F_{k-1} \\
>         & < \left(\dfrac{5}{3}\right)^k + \left(\dfrac{5}{3}\right)^{k-1} \\
>         & = \dfrac{24}{25}\left(\dfrac{5}{3}\right)^{k+1} \\
>         & < \left(\dfrac{5}{3}\right)^{k+1}
> \end{aligned}
> $$
>     故 $\mathcal(k+1) = \mbox{True}$
>
> 根据第二类数学归纳法可得：$\forall n \in \mathbb{N}^{*}, \mathcal{P}(n) = \mbox{True}$。<br/>
> 即：$\forall n \in \mathbb{N}^{*}, F_n < \left(\dfrac{5}{3}\right)^n$<br/>
> 证毕。
* **反证法**<br/>
例：求证：存在无穷多个素数。
> **证明**：假设结论不成立，即存在有限个素数。<br/>
> 设一共有 $n_0$ 个素数。将所有素数从小到大排列得到的素数列记为 $\left(P_n\right)_{n\in[\![1, n_0]\!]}$<br/>
> 则 $P_{n_0}$ 为最大的素数<br/>
> 考虑自然数 $N = P_1P_2 \cdots P_{n_0} + 1$<br/>
> 显然 $N > P_{n_0}$<br/>
> 故 $N$ 不是素数。<br/>
> 另一方面，用所有的素数 $P_1, P_2, \cdots, P_{n_0}$ 去除 $N$，都不能整除，会有余数 $1$<br/>
> 故 $N$ 是素数。<br/>
> 矛盾！<br/>
> 故假设不成立。<br/>
> 故确实存在无穷多个素数。

# 递归简论
递归的基本准则：
1. 基准情形（base case）：必须要存在一种情形，不需要递归就可以求解
2. 不断推进（making progress）：计算非基准情形时，递归调用一定要朝着基准情形推进
3. 设计法则（design rule）：在设计递归时，要假设每次递归调用都能按照期望运行并返回正确的结果
4. 合成效益法则（compound interest rule）：切勿在不同的递归调用中计算同一个实例（影响效率而不影响正确性）
例如递归求解斐波那契数列的算法是不好的，违反了第四条：
```java
public static double Fibonacci(int n)
{
	if(n <= 1)
		return 1;
	return Fibonacci(n-1) + Fibonacci(n-2);
} 
```

# Java 特性
略

# 习题
1. 编写一个程序解决选择问题。令 $k = \lfloor\frac{n}{2}\rfloor$，画图显示程序对不同规模输入的运行时间。<br/>
程序见 [KSelector/sort_select2](Code/k_selection/src.html/KSelector.java.html)<br/>
不同规模输入的运行时间图见下图<br/>
![不同规模输入的 k 选择问题求解时间](Figures/time_elaspe.png)
2. 编写一个求解字谜游戏问题的程序。
7. 证明下列公式：
    1. $\forall x > 0, \log_2x < x$
    > **证明**：$\forall x > 0$，定义 $f(x) = x-\log_2x$  
    > 则 $\forall x > 0, f'(x) = 1-\dfrac{1}{x\ln 2} = \dfrac{x-\log_2\mathrm{e}}{x}$  
    > 故 $f$ 在 $\left[0, \log_2\mathrm{e}\right]$ 上递减，在 $[\log_2\mathrm{e}, +\infty]$ 上递增。  
    > 故 $f$ 的最小值为 $f(\log_2\mathrm{e}) = \log_2\mathrm{e}-\log_2\left(\log_2\mathrm{e}\right)$  
    > 由于 $\mathrm{e} > 2, \log_2e < 2$ 且 $\log_2x$ 为增函数，  
    > 故 $\log_2\mathrm{e} > \log_2\left(\log_2\mathrm{e}\right)$  
    > 故 $\forall x > 0, f(x) \geqslant \log_2e-\log_2\left(\log_2e\right) > 0$  
    > 故 $\forall x > 0, \log_2x < x$
8. 计算下列各和：
    1. $\sum\limits_{i=0}^{\infty}\dfrac{1}{4^i}$
    2. $\sum\limits_{i=0}^{\infty}\dfrac{i}{4^i}$
    3. $\sum\limits_{i=0}^{\infty}\dfrac{i^2}{4^i}$
    4. $\sum\limits_{i=0}^{\infty}\dfrac{i^n}{4^i}$

    解：

    1. $\sum\limits_{i=0}^{\infty}\dfrac{1}{4^i} = \dfrac{1}{1-\dfrac{1}{4}} = \dfrac{4}{3}$
    2. $\sum\limits_{i=0}^{\infty}\dfrac{i}{4^i} = \dfrac{\frac{1}{4}}{\left(1-\frac{1}{4}\right)^2} = \dfrac{4}{9}$
    3. 
    > 记 $S(x) = \sum\limits_{i=0}^{\infty}i^2x^i$，记 $f_i(x) = x^{i+2}$  
    > 则
    > $$
    > \left\{
    > \begin{aligned}
    > & f_i'(x) = (i+2)x^{i+1}\\
    > & f_i''(x) = (i^2+3i+2)x^i
    > \end{aligned}
    > \right.
    > $$
    > 故
    > $$
    > \begin{aligned}
    > \sum_{i=0}^{\infty}f_i''(x) & = \sum_{i=0}^{\infty}\left(i^2x^i + 3ix^i + 2x^i\right)\\
    >                        & = \sum_{i=0}^{\infty}i^2x^i + 3\sum_{i=0}^{\infty}ix^i + 2\sum_{i=0}^{\infty}x^i\\
    >                        & = S(x) + \dfrac{3x}{(x-1)^2} + 2\dfrac{1}{1-x}\\
    >                        & = S(x) + \dfrac{x + 2}{(x-1)^2}
    > \end{aligned}
    > $$
    > 另一方面，由于
    > $$
    > \sum\limits_{i=0}^{\infty}f_i(x) = \dfrac{x^2}{1-x}
    > $$
    > 故
    > $$
    > \sum\limits_{i=0}^{\infty}f_i'(x) = \dfrac{2x-x^2}{(1-x)^2}
    > $$
    > 故
    > $$
    > \sum\limits_{i=0}^{\infty}f_i''(x) = \dfrac{2}{(1-x)^3} = S(x) + \dfrac{x + 2}{(x-1)^2}
    > $$
    > 故
    > $$
    > S(x) = \dfrac{2}{(1-x)^3} - \dfrac{x + 2}{(x-1)^2} = \dfrac{x(x+1)}{(1-x)^3}
    > $$
    > 原式 $= S\left(\dfrac{1}{4}\right) = \dfrac{20}{27}$
    4. $\sum\limits_{i=0}^{\infty}\dfrac{i^n}{4^i} = \mathrm{Li}_{-n}\left(\dfrac{1}{4}\right)$
9. 求 $\sum\limits_{i = \lfloor n/2 \rfloor}^n\dfrac{1}{i}$ 在正无穷处的等价
	> **解**：
	> $$
	> \begin{aligned}
	> \sum_{i = \lfloor n/2 \rfloor}^n\dfrac{1}{i} & = \sum_{i = 0}^{n}\dfrac{1}{i} - \sum_{i = 0}^{\lfloor n/2 \rfloor-1}\dfrac{1}{i}\\
	>                                  & \sim \ln n - \ln\left(\lfloor n/2 \rfloor-1\right)\\
	>                                  & = \ln\left(\dfrac{n}{\lfloor n/2 \rfloor-1}\right)\\
	>                                  & \sim \ln 2 
	> \end{aligned}
	> $$