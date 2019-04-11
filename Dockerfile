from ubuntu:latest

RUN apt update     \
 && apt install -y \
    man    \
    gcc    \
    strace \
    gdb    \
    procps \
    htop   \
    vim    \
    git    \
    curl   \
    zsh    \
 && sh -c "$(curl -fsSL https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh)"

RUN echo 'set -o vi' >> /root/.zshrc

WORKDIR /bob-ross
