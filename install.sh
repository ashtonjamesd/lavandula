#!/bin/bash

set -e

REPO="ashtonjamesd/lavandula"
INSTALL_DIR="/usr/local"
LAVANDULA_LIB_DIR="/usr/local/lib/lavandula"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

command_exists() {
    command -v "$1" >/dev/null 2>&1
}

check_dependencies() {
    if ! command_exists gcc; then
        log_error "GCC is required but not installed. Please install GCC first."
        exit 1
    fi

    if ! command_exists make; then
        log_error "make is required but not installed. Please install make first."
        exit 1
    fi

    if ! command_exists git; then
        log_error "git is required but not installed. Please install git first."
        exit 1
    fi

    # temporary fix, does not work on MacOS currently
    # if [ ! -f "/usr/include/sqlite3.h" ]; then
    #     log_error "Development tools for the sqlite3 is required but not installed. Please install sqlite3 devel first."
    #     exit 1
    # fi
}

install_repository() {
    log_info "Installing repository to ${LAVANDULA_LIB_DIR}..."

    sudo rm -r "${LAVANDULA_LIB_DIR}"

    sudo mkdir -p "${LAVANDULA_LIB_DIR}"
    
    sudo cp -r "." "${LAVANDULA_LIB_DIR}/"
    
    if [[ -z "$OSTYPE" && "$OSTYPE" == "darwin"* ]]; then
        sudo chown -R root:wheel "${LAVANDULA_LIB_DIR}"
    else
        sudo chown -R root:root "${LAVANDULA_LIB_DIR}"
    fi

    sudo chmod -R 755 "${LAVANDULA_LIB_DIR}"
    
    log_success "Repository installed to ${LAVANDULA_LIB_DIR}"
}

build_project() {
    log_info "Building Lavandula..."
    make
    log_success "Build completed"
}

install_project() {
    log_info "Installing Lavandula to ${INSTALL_DIR}..."

    if [ ! -f build/lavu ]; then
        log_error "Built binary 'lavu' not found. Build may have failed."
        exit 1
    fi

    if [ ! -d "${INSTALL_DIR}/bin" ]; then
        mkdir -p "${INSTALL_DIR}/bin"
    fi

    sudo cp build/lavu "${INSTALL_DIR}/bin/"
    sudo chmod +x "${INSTALL_DIR}/bin/lavu"

    log_success "Installation completed"
}

verify_installation() {
    if command_exists lavu; then
        log_success "Lavandula installed successfully!"
    else
        log_error "Lavandula installation failed."
        exit 1
    fi
}

show_usage() {
    echo ""
    log_success "🎉 Lavandula installation completed!"
    echo ""

    echo -e "${BLUE}Quick Start:${NC}"
    echo " lavu new my-project    # Create a new project"
    echo " cd my-project"
    echo " lavu run               # Run your project"
    echo ""

    echo -e "${BLUE}Documentation:${NC}"
    echo " GitHub: https://github.com/$REPO"
    echo ""
}

main() {
    echo -e "${BLUE}"
    echo "  _                                _       _       "
    echo " | |                              | |     | |      "
    echo " | |     __ ___   ____ _ _ __   __| |_   _| | __ _ "
    echo " | |    / _\` \ \ / / _\` | '_ \ / _\` | | | | |/ _\` |"
    echo " | |___| (_| |\ V / (_| | | | | (_| | |_| | | (_| |"
    echo " |______\__,_| \_/ \__,_|_| |_|\__,_|\__,_|_|\__,_|"
    echo -e "${NC}"

    echo "Installing Lavandula Web Framework..."
    echo ""

    check_dependencies
    build_project
    install_project
    install_repository

    show_usage
}

main "$@"