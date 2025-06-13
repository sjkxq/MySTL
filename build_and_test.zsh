#!/bin/zsh

# 设置错误处理
set -e

# 定义颜色输出函数
print_info() {
    echo "\033[0;34m[INFO]\033[0m $1"
}

print_success() {
    echo "\033[0;32m[SUCCESS]\033[0m $1"
}

print_error() {
    echo "\033[0;31m[ERROR]\033[0m $1"
}

print_warning() {
    echo "\033[0;33m[WARNING]\033[0m $1"
}

# 默认参数
BUILD_TYPE="Debug"
BUILD_DIR="build"
CLEAN=false
RUN_TESTS=true
JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)

# 显示帮助信息
show_help() {
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  -h, --help          显示帮助信息"
    echo "  -c, --clean         清理构建目录"
    echo "  -r, --release       使用Release模式构建（默认为Debug）"
    echo "  -d, --debug         使用Debug模式构建（默认）"
    echo "  -b, --build-dir DIR 指定构建目录（默认为'build'）"
    echo "  -j, --jobs N        指定并行构建任务数（默认为可用CPU核心数）"
    echo "  --no-tests          跳过测试"
    echo ""
}

# 解析命令行参数
parse_args() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_help
                exit 0
                ;;
            -c|--clean)
                CLEAN=true
                shift
                ;;
            -r|--release)
                BUILD_TYPE="Release"
                shift
                ;;
            -d|--debug)
                BUILD_TYPE="Debug"
                shift
                ;;
            -b|--build-dir)
                BUILD_DIR="$2"
                shift 2
                ;;
            -j|--jobs)
                JOBS="$2"
                shift 2
                ;;
            --no-tests)
                RUN_TESTS=false
                shift
                ;;
            *)
                print_error "未知选项: $1"
                show_help
                exit 1
                ;;
        esac
    done
}

# 检查必要的命令是否存在
check_requirements() {
    print_info "检查环境要求..."
    
    local required_commands=("cmake" "make")
    
    for cmd in $required_commands; do
        if ! command -v $cmd &> /dev/null; then
            print_error "$cmd 未安装，请先安装必要的工具"
            exit 1
        fi
    done
    
    # 检查是否有ninja，如果有则优先使用
    if command -v ninja &> /dev/null; then
        print_info "检测到Ninja构建系统，将使用Ninja替代Make"
        USE_NINJA=true
    else
        USE_NINJA=false
    fi
    
    print_success "环境检查通过"
}

# 清理构建目录
clean_build_dir() {
    if [[ $CLEAN == true ]]; then
        print_info "清理构建目录: $BUILD_DIR"
        if [[ -d "$BUILD_DIR" ]]; then
            rm -rf "$BUILD_DIR"
            print_success "构建目录已清理"
        else
            print_info "构建目录不存在，无需清理"
        fi
    fi
}

# 配置项目
configure_project() {
    print_info "配置项目 (构建类型: $BUILD_TYPE)..."
    
    # 创建构建目录（如果不存在）
    if [[ ! -d "$BUILD_DIR" ]]; then
        mkdir -p "$BUILD_DIR"
    fi
    
    # 进入构建目录
    cd "$BUILD_DIR"
    
    # 配置CMake项目
    if [[ $USE_NINJA == true ]]; then
        cmake -G Ninja -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
    else
        cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
    fi
    
    # 返回原目录
    cd ..
    
    print_success "项目配置完成"
}

# 构建项目
build_project() {
    print_info "开始构建项目..."
    
    # 进入构建目录
    cd "$BUILD_DIR"
    
    # 构建项目
    if [[ $USE_NINJA == true ]]; then
        ninja -j $JOBS
    else
        make -j $JOBS
    fi
    
    # 返回原目录
    cd ..
    
    print_success "项目构建完成"
}

# 运行测试
run_tests() {
    if [[ $RUN_TESTS == true ]]; then
        print_info "开始运行测试..."
        
        # 进入构建目录
        cd "$BUILD_DIR"
        
        # 运行ctest
        ctest --output-on-failure
        
        # 返回原目录
        cd ..
        
        print_success "测试完成"
    else
        print_info "跳过测试"
    fi
}

# 主函数
main() {
    print_info "开始构建和测试流程..."
    
    # 解析命令行参数
    parse_args "$@"
    
    # 检查环境要求
    check_requirements
    
    # 清理构建目录（如果需要）
    clean_build_dir
    
    # 配置项目
    configure_project
    
    # 构建项目
    build_project
    
    # 运行测试
    run_tests
    
    print_success "所有任务已完成!"
}

# 执行主函数，传递所有命令行参数
main "$@"