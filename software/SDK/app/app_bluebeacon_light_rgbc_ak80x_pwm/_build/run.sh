#!/bin/bash
echo "////////////////编译////////////////////"
cdk-make.exe  --workspace="soc_test.cdkws"   --command="clean"  --project="soc_test"  --config="BuildSet"
cdk-make.exe  --workspace="soc_test.cdkws"   --command="build"  --project="soc_test"  --config="BuildSet"

echo " "
echo "////////////////栈调用//////////////////"
echo "----------------------------------------"
cat Lst/soc_test.htm | grep -e "Max Depth" -e "Call Chain" | sed "s:<..>::g;s:</..>::g;s:\[Stack\]::g;s:&rArr;:->:g"
echo "----------------------------------------"


echo " "
echo "////////////////FLASH占用-ZI段//////////"
echo "----------------------------------------"
flash_start=`cat Lst/soc_test.map | grep -n "Region ER_ROM1" | cut -f1 -d:`
flash_end=`cat Lst/soc_test.map | grep -n "Region LR_ROM1" | cut -f1 -d:`
sed -n $flash_start,$flash_end"p" Lst/soc_test.map | sed "s:\(.*\t\).*\.o:\1:" 


# https://blog.csdn.net/wxh0000mm/article/details/80364701
echo " "
echo "////////////////资源占用////////////////"
echo "> text 段用于存放程序代码的区域, 编译时确定, 只读. 包含的内容是代码和常量"
echo "> data 段已初始化的全局变量保存在.data 段中"
echo "> bss  段未初始化的全局变量保存在.bss 段中"
echo "> dec/hex = text + data + bss"
echo "> flash = text + data"
echo "> ram = bss + data"

# https://blog.csdn.net/ylzmm/article/details/79233560
echo " "
echo "> Code: 不仅仅包含了代码的大小。其中还包括了inline data(内联数据)，比如literal pools(文字池)
    和short strings(短字符串)"
echo "> RO-data: 表示程序中的只读常量数据。比如const类型的数据 (除inline data之外的只读数据)。"
echo "> RW-data: 表示可读写变量的大小,也就是说是程序中已经初始化的变量大小"
echo "> ZI-data: 表示程序中未初始化的变量大小。就是程序中已经定义但是初始化值为0或者
   是没有初始化赋值的变量(编译器会默认把没有赋值的变量自动赋值为0)"
echo "> ROM size: 也就是下载到单片机FLASH中的数据为  Code +RO-data +RW-data 和的大小"
echo "> RAM size: 程序运行时所需要的大小空间为 RW-data+ ZI-data"

echo " "
echo "----------------------------------------"
cat crt0.S | grep -e "#define[[:space:]]*StackSize"
cat  Lst/soc_test.map | grep -e "^Total"
echo "----------------------------------------"

#  text    data     bss     dec     hex filename
#  13280      60    1324   14664    3948 C:/User

