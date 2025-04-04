if get_option('zsh-completions')
  zsh_files = files(
    'zsh/_mfw',
  )
  zsh_install_dir = join_paths(datadir, 'zsh', 'site-functions')

  install_data(zsh_files, install_dir: zsh_install_dir)
endif
