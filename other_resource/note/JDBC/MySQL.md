# MySQL

# 配置 MySQL
在安装 MySQL 后，无法直接启动 MySQL，还需进行配置、安装服务、初始化、启动服务、连接服务等操作。具体步骤如下：

1. 将 `MySQL/install/path/bin` 添加到环境变量 `PATH`
2. 进行配置：在 `MySQL/install/path` 下新建一个 `my.ini` 文件，里面填入如下内容：
```ini
[mysql]
default-character-set = utf8

[mysqld]
port = 3306
basedir = "MySQL/install/path"
datadir = "MySQL/install/path/data"
max_connections = 200
character-set-server = utf8
default-storage-engine = INNODB
```
3. 安装服务、初始化、启动服务：打开 cmd 分别执行如下命令：  
```batch
mysqld --install
mysqld --initialize
net start mysql
```
4. 连接服务：运行 `mysql -uroot -p` 后提示你输入密码，初始密码在 `MySQL/install/path/data/computer_name.err` 中类似下面的一行中找到：  
```log
2021-03-21T09:32:19.155308Z 6 [Note] [MY-010454] [Server] A temporary password is generated for root@localhost: random_password
```
5. 修改密码：连接成功后，输入 `exit` 断开连接，并运行命令 `mysqladmin -u root -p password new_password` 将初始密码修改为 `new_password`
6. 重新连接：cmd 运行 `mysql -uroot -p` 后输入你的新密码即可。

# 查询外的所有操作
* 查看已存在的用户：
```sql
SELECT host, user
FROM user;
```
* 添加新用户：  
```sql
INSERT INTO user(host, user, password[, priv1, priv2, ...])
VALUES('host_name', 'user_name', 'your_password'[, 'Y/N', 'Y/N', ...]);
```  
其中 `'Y/N'` 请替换为 `'Y'` 或 `'N'`，代表是否有这项权限；而 `priv` 可以为：
	* `Select_priv`：查询权限
	* `Insert_priv`：插入记录权限
	* `Update_priv`：更改记录权限
	* `Delete_priv`：删除记录权限
	* `Create_priv`：创建新表权限
	* `Drop_priv`：删除表权限
	* `Reload_priv`：
	* `Shutdown_priv`：关闭 MySQL 服务的权限
	* `Process_priv`
	* `File_priv`
	* `Grant_priv`：为其他用户分配权限的权限
	* `References_priv`
	* `Index_priv`
	* `Alter_priv`：添加删除更改列的权限
* 删除用户：
```sql
DELETE FROM user WHERE user = 'user_name';
```
* 查看 MySQL 下的所有数据库：
```sql
SHOW DATABASES;
```
* 开始使用某一个数据库：
```sql
USE database_name;
```
* 查看当前数据库下的所有表：
```sql
SHOW TABLES;
```
* 查看某个表的全部内容：
```sql
SELECT * FROM table_name;
```
* 查看某个表的所有列的属性：
```sql
SHOW COLUMNS FROM table_name;
```
* 创建数据库：
```sql
CREATE DATABASE database_name;
```
* 删除数据库：
```sql
DROP DATABASE database_name;
```
* 创建表：
```sql
CREATE TABLE IF NOT EXISTS table_name
(
	column_name1 type1(size1) constrain1,
	column_name2 type2(size2) constrain2,
	...,
	PRIMARY KEY(column_name1)
);
```
其中：
    * `type` 为数据类型
    * `size` 为指定数据占有的字节数
    * `constrain` 为约束，约束可以是如下值：
        * `NOT NULL`：插入行时不能不填这列
        * `UNIQUE`：这列的值在整张表中必须唯一
        * `AUTO_INCREMENT`：插入时如果不填这列，这列的值会自增
        * `DEFAULT value`：插入时如果不填这列，这列的值就为 `value`
    * `PRIMARY KEY` 指定主键，相当于给列加上 `NOT NULL` 和 `UNIQUE` 约束。
* 删除表：
```sql
DROP TABEL table_name;
```
* 添加列
```sql
ALTER TABLE table_name
ADD column_name type(size) constrain;
```
* 删除列
```sql
ALTER TABLE table_name
DROP column_name;
```
* 修改列名称和属性
```sql
ALTER TABLE table_name
CHANGE old_column_name new_column_name type(size) constrain;
```
* 添加行：  
插入一行数据时可以提供完整的列信息，如下所示：
```sql
INSERT INTO table_name
VALUES(data1, data2, data3);
```
也可以仅提供指定列数据，如下所示：
```sql
INSERT INTO table_name (column_name1, column_name2, column_name3)
VALUES(data1, data2, data3);
```
当插入一条重复的记录时，忽略之的方法是：
```sql
INSERT IGNORE INTO table_name (column_name1, column_name2, column_name3)
VALUES(data1, data2, data3);
```
* 删除行
```sql
DELETE FROM table_name
WHERE condition;
```
如果没有提供 `WHERE condition` 则表中所有行均会被删除。
* 修改行内容
```sql
UPDATE table_name
SET column_name1 = value1, column_name2 = value2
WHERE condition;
```
如果没有提供 `WHERE condition` 则所有行均会被更新。

> **注意**：
> 
> 1. 数据库名、表名、列名等用户定义的信息，在存储时大写字母均会被转为小写。所以请不要采用驼峰命名法。
> 2. 字符串类型的值用单引号。

# 查询

## 单表查询
* 查看某个表的全部内容：
```sql
SELECT *
FROM table;
```
* 从表中选出指定列：
```sql
SELECT column1, column2
FROM table;
```
* 从表中选出指定列以及满足条件的行：
```sql
SELECT column1, column2
FROM table
WHERE condition;
```
其中 `condition` 中可用的操作符有：

| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;操作符&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; | 描述 |
|--------|------|
| `value1 = value2` | 检测 `value1` 是否等于 `value2`，如果相等返回 `true` |
| `value1 <> value2` |	检测 `value1` 是否不等于 `value2`，如果不相等返回 `true` |
| `value1 > value2` | 检测 `value1` 是否大于 `value2`， 如果大于则返回 `true` |
| `value1 < value2` | 检测 `value1` 是否小于 `value2`，如果小于则返回 `true` |
| `value1 >= value2` | 检测 `value1` 是否大于等于 `value2`，如果是则返回 `true` |
| `value1 <= value2` | 检测 `value2` 是否小于等于 `value2`，如果是则返回 `true` |
| `str1 LIKE str2` | 检测字符串 `str1` 是否与字符串 `str2` 相似，如果相似则返回 `true`。<br/>在使用 `LIKE` 时，字符串 `str2` 中可以包含一些通配符：<br/>`_` 代表可以匹配任意单个字符，<br/>`%` 代表可以匹配任意个任意字符。 |
| `str1 REGEXP regx` | 检测字符串 `str1` 中是否含有正则表达式 `regx` 的匹配，如果有则返回 `true`。 |
| `value1 BETWEEN value2 AND value3` | 检测 `value1` 是否在 `value2` 和 `value3` 之间，如果是则返回 `true`。|
| `value IN (value1, value2, ...)` | 检测 `value` 是否是 `(value1, value2, ...)` 的其中之一，如果是则返回 `true`。|
| `condition1 AND condition2` | 逻辑与，当条件 `condition1` 和条件 `condition2` 都满足时返回 `true` |
| `condition1 OR condition2` | 逻辑或，当条件 `constrain1` 和条件 `constrain2` 有一个满足就返回 `true` |
| `NOT condition` | 逻辑非，当条件 `condition` 不满足时返回 `true` |

* 去除查询结果中重复的行
```sql
SELECT DISTINCT *
FROM table
WHERE condition
```
* 只显示查询结果的前 `N` 行
```sql
SELECT *
FROM table
WHERE condition
LIMIT N
```
* 查询后对行排序
```sql
SELECT column1, column2
FROM table
WHERE condition
ORDER BY column1, column2 DESC
```
将对查询结果中的行按 `column1` 升序排序，`column1` 相同的，将按 `column2` 降序排序。

### 聚合
```sql
SELECT aggregate_function(column)
FROM table_name
WHERE condition
```
将得到某列的统计信息，即统计所有满足条件 `condition` 的行中的 `column` 变成一个值，在结果中便只会得到一行。其中 `aggregate_function` 有：

* `COUNT`：计算满足条件的记录共几行；
* `SUM`：在所有满足条件的记录中，对该列求和；
* `AVG`：在所有满足条件的记录中，对该列求平均值；
* `MAX`：在所有满足条件的记录中，该列值最大的；
* `MIN`：在所有满足条件的记录中，该列值最小的；

可以说，聚合函数把筛选出来的所有行聚合成了一行。

当我们不想把所有行聚合成一行，而是想把某些行聚合成一行，另一些行聚合成另外一行，这样分组聚合，怎么办？可以添加 `GROUP BY` 来实现：
```sql
SELECT column1, aggregate_function(column2)
FROM table_name
WHERE condition
GROUP BY column1
```
这时 `column1` 相同的行被聚合成一行，`column1` 相同的另外一些行又被聚合成了另外一行，实现了分组聚合的目的。

可以说，分组聚合后会得到好多行数据。

当我们想再次筛选分组聚合后的行呢？（在 `WHERE` 处筛不掉的，因为 `WHERE` 只能看聚合前的单行数据是否满足条件，而我们现在想筛由多行数据聚合后的记录）可以添加 `HAVING` 来实现：
```sql
SELECT column1, aggregate_function1(column2)
FROM table_name
WHERE condition
GROUP BY column1
HAVING aggregate_function2(column2) > 5
```
这就又筛选掉了聚合后的一些行。

## 联表查询

### 上下拼接
如果从两张表中查询的结果具有相同的列名和列数据类型，可将两次查询结果上下拼接：
```sql
SELECT *
FROM table1
WHERE condition1
UNION
SELECT *
FROM table2
WHERE condition2
```
直接使用 `UNION` 会过滤掉重复的行。可以使用 `UNION ALL` 使之保留重复行
```sql
SELECT *
FROM table1
WHERE condition1
UNION ALL
SELECT *
FROM table2
WHERE condition2
```

### 左右拼接
#### 交叉连接
如果要使 `FROM` 后的表为两张表的笛卡尔积，直接用逗号 `,` 隔开两张表即可。
```sql
...
FROM table1, table2
WHERE table1.column1 = table2.column2 AND other_condition
...
```
这样的连接称为交叉连接。

#### 内连接
交叉连接的效率极低，因为 `table1, table2` 将返回一张行数过多的表，其行数是 `table1` 的行数与 `table2` 的行数之积。随后从这张大表中选择满足条件的行。这是不明智的选择。内连接则可以实现不生成完整的笛卡尔积，只拼接满足条件的行，然后把这些行组成一张表。与上面代码内容等价的内连接方案是：
```sql
...
FROM table1 INNER JOIN table2
ON table1.column1 = table2.column2
WHERE other_condition
...
```
虽然效果等价，但大大提高了查询速度，减少了内存占用。

#### 外连接
##### 左连接
左连接的意思是，首先显示左边的表，随后把右表中满足连接条件的的行拼到左表对应的行上。左表中缺失的那些列填 `NULL`
```sql
...
FROM table1 LEFT JOIN table2
ON table1.column1 = table2.column2
WHERE other_condition
...
```
##### 右连接
右连接的意思是，首先显示右边的表，随后把左表中满足连接条件的的行拼到右表对应的行上。右表中缺失的那些列填 `NULL`
```sql
...
FROM table1 RIGHT JOIN table2
ON table1.column1 = table2.column2
WHERE other_condition
...
```
##### 全连接
全连接的意思是，首先显示左右两张表，随后把左右表中满足连接条件的行拼在一起。左右表中缺失的那些列填 `NULL`
```sql
...
FROM table1 FULL JOIN table2
ON table1.column1 = table2.column2
WHERE other_condition
...
```

# 数据导出
## 导出到文本文件
在查询语句后直接接 `INTO OUTFILE 'file_name'` 即可，例如
```sql
SELECT *
FROM table1
WHERE condition
INTO OUTFILE 'test.txt';
```

# 索引
# 事务
# 触发器
# 存储过程
# 函数