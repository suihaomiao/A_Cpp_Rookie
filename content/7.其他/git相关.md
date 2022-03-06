# git相关操作
## 背景
git是企业常用的版本控制软件，需要掌握常用的操作，例如：
克隆，拉取，推送，切换分支等
以下会按照工作的经验，来介绍git

## 拉取代码仓库
编码的第一件事，就是拉取仓库；
``` shell
git clone xxx
# xxx- 表示代码仓库地址，通常企业会搭建自己的# git网站。
# 还可以加-b 分支名来拉取仓库并切换到指定分支。
```

## 需求编码
在接到需求或者解决bug时，需要从主干的分支下创建自己的分支，然后实施修改。
``` shell
# 创建自己的分支
git branch 自己的分支名
# 切换到自己的分支
git checkout 自己的分支名
```

## 修改完成
完成修改，验证功能无误后，需要合入主干。
这里，大公司会需要在网站上进行合入（配合一系列门禁之类），我们只是测试而已。
``` shell
# 处于自己的分支，合入主干分支
git merge 主干分支
```

## 解决冲突
在合入主干的时候，有时候会碰到冲突的时候，这个时候需要解决冲突。
冲突发生的原因在于，不同的人在同一个位置，进行了修改，git无法判断出哪一个是需要保留的。
这里，无法模拟两个人同时修改，即转换思路为同一个人在不同分支下进行了同样的修改，具体如下：
1. 首先在main分支下对输出的helloworld 修改为old cout等字样
2. 然后在suihaomiao_branch分支下降helloworld 修改为new cout等字样。
3. 最后将main合入suihaomiao_branch分支，冲突产生，此时需要我们手动解决冲突
![冲突产生](https://github.com/suihaomiao/A_Cpp_Rookie/blob/main/images/git_%E5%86%B2%E7%AA%81%E6%8F%90%E7%A4%BA.png)
![解决冲突](https://github.com/suihaomiao/A_Cpp_Rookie/blob/main/images/git_%E8%A7%A3%E5%86%B3%E5%86%B2%E7%AA%81.png)
4. 在最后解决完成冲突后，再次提交，这个时候可以使用：
![解决冲突后](https://github.com/suihaomiao/A_Cpp_Rookie/blob/main/images/git_%E8%A7%A3%E5%86%B3%E5%86%B2%E7%AA%81%E5%90%8E%E5%86%8D%E6%AC%A1commit.jpg)
``` shell
# --no-edit会自动帮你添加注释，为解决冲突
git commit --no-edit
```
## 不同分支之间同步
在实际开发中，可能会遇到同一个修改需要同步到不同分支上（对应不同产品）；
因为开发流程应该是，在不同的分支下，分别拉取分支，然后修改，最后合入到目标分支上去。
那么如果有不同的产品，对应不同的分支，那岂不是要自己手动在两个分支下进行同样的修改？
实际上是可以不用的（像我这种菜逼一开始就是手动修改，非常有可能会出错）。
使用git cherry-pick，这个功能网上讲的很多，大概就是在当前的分支下创建一个新的commit，这个commit可以是其他分支下的。
``` shell
# commit编号可以通过git log查看
git cherry-pick commit编号

```
