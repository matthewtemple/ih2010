if [ "$DISPLAY" != "" ]; then
    xset b off >/dev/null 2>&1
fi
setterm -blank 0 >/dev/null 2>&1
setterm -blength 0 2 >/dev/null 2>&1

export PS1="(\u@\h \w) "
export IH=$HOME/ih
export PATH="$IH/bin/user/matthew:$IH/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/X11R6/bin:$PATH"
export LD_LIBRARY_PATH=/usr/lib:/usr/lib64:/usr/local/lib:$LD_LIBRARY_PATH
export EDITOR=vi

alias c="clear"
alias cmx="c; mx"
alias cs="ih-stats"
alias df="df -h"
alias e="emacs -nw"
alias hc="hg commit"
alias hp="hg push \"https://Matthew Van Temple@inhesion.kilnhg.com/Repo/inhesion/Group/ih\""
alias hs="hg status"
alias i="irssi"
alias ls="ls -Fh"
alias la="ls -Fha"
alias ll="ls -Flh"
alias lal="ls -Flha"
alias ltr="ls -Flhtr"
alias latr="ls -Flhatr"
alias sl="ls"
alias m="make"
alias mc="make clean"
alias md="make demo"
alias mdb="make 2>&1 | less"
alias mr="make release"
alias mt="make test"
alias mtag="ih-make-etags"
alias mts="make tests"
alias mx="make clean; make"
alias rf="ih-find"
alias rmtilde="ih-rm-tilde"
alias s="screen -h 2048"
alias u="uptime"
alias which-ubuntu="lsb_release -a"
alias x="exit"
alias v="m-valgrind"
