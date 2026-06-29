@echo off
chcp 65001 >nul
echo ========================================
echo  图片声明生成器
echo ========================================
echo.
echo 当前目录: %CD%
echo.

REM 检查是否有图片
set count=0
for /r . %%f in (*.c) do (
    set /a count+=1
)

if %count%==0 (
    echo ? 没有找到图片文件！
    echo 请确认：
    echo 1. bat文件是否在项目根目录
    echo 2. 图片扩展名是否正确
    echo 3. 文件名大小写是否匹配
    pause
    exit
)

echo ? 找到 %count% 个图片文件
echo 正在生成声明...

(
for /r . %%f in (*.c) do (
    echo LV_IMAGE_DECLARE(%%~nf^)^;
)
) > image_declares.h

echo.
echo ? 成功生成 image_declares.h
echo 共 %count% 个声明
echo.
pause