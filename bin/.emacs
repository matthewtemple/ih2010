(setq ih-use-abbrevs nil)
(if (file-exists-p "~/.emacs_ih") (load "~/.emacs_ih"))
(if (file-exists-p "~/.emacs_custom") (load "~/.emacs_custom"))
(if ih-use-abbrevs (load "~/.emacs_ih_abbrevs"))
