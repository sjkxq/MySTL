#!/bin/bash

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 显示帮助信息
show_help() {
    echo -e "${BLUE}MySTL 代码格式化工具${NC}"
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  -h, --help      显示此帮助信息"
    echo "  -c, --check     只检查格式问题，不修改文件"
    echo "  -f, --fix       修复格式问题（默认）"
    echo ""
    echo "示例:"
    echo "  $0 --check      # 检查所有文件的格式问题"
    echo "  $0 --fix        # 修复所有文件的格式问题"
    echo "  $0              # 默认修复所有文件的格式问题"
}

# 检查 clang-format 是否已安装
check_clang_format() {
    if ! command -v clang-format &> /dev/null; then
        echo -e "${RED}错误: clang-format 未安装${NC}"
        echo "请安装 clang-format 后再运行此脚本"
        echo "安装命令示例:"
        echo "  Ubuntu/Debian: sudo apt-get install clang-format"
        echo "  macOS: brew install clang-format"
        echo "  CentOS/RHEL: sudo yum install clang-format"
        exit 1
    fi
}

# 查找所有 C++ 源文件和头文件
find_cpp_files() {
    find include tests -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.cc" \) 2>/dev/null
}

# 检查模式 - 只检查格式问题，不修改文件
check_format() {
    echo -e "${BLUE}检查代码格式问题...${NC}"
    
    local has_issues=0
    local files=$(find_cpp_files)
    local file_count=$(echo "$files" | wc -l)
    
    if [ -z "$files" ]; then
        echo -e "${YELLOW}警告: 未找到任何 C++ 源文件或头文件${NC}"
        return 0
    fi
    
    echo -e "找到 ${file_count} 个文件需要检查"
    
    for file in $files; do
        diff=$(clang-format "$file" | diff -u "$file" -)
        if [ -n "$diff" ]; then
            echo -e "${RED}格式问题: ${file}${NC}"
            has_issues=1
        fi
    done
    
    if [ $has_issues -eq 0 ]; then
        echo -e "${GREEN}所有文件格式正确!${NC}"
        return 0
    else
        echo -e "${YELLOW}发现格式问题。运行 '$0 --fix' 修复这些问题。${NC}"
        return 1
    fi
}

# 修复模式 - 修复格式问题
fix_format() {
    echo -e "${BLUE}修复代码格式问题...${NC}"
    
    local files=$(find_cpp_files)
    local file_count=$(echo "$files" | wc -l)
    
    if [ -z "$files" ]; then
        echo -e "${YELLOW}警告: 未找到任何 C++ 源文件或头文件${NC}"
        return 0
    fi
    
    echo -e "找到 ${file_count} 个文件需要格式化"
    
    for file in $files; do
        echo -e "格式化: ${file}"
        clang-format -i "$file"
    done
    
    echo -e "${GREEN}所有文件已格式化!${NC}"
    return 0
}

# 主函数
main() {
    # 检查 clang-format 是否已安装
    check_clang_format
    
    # 解析命令行参数
    case "$1" in
        -h|--help)
            show_help
            exit 0
            ;;
        -c|--check)
            check_format
            exit $?
            ;;
        -f|--fix|"")
            fix_format
            exit $?
            ;;
        *)
            echo -e "${RED}错误: 未知选项 $1${NC}"
            show_help
            exit 1
            ;;
    esac
}

# 执行主函数
main "$@"