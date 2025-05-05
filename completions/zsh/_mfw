#compdef mfw

# Usage: mfw <filename> <command> [args...]

_arguments -C \
  '1: :->first' \
  '2: :->second' \
  '*:: :->rest'

case $state in
  first)
    _files
    ;;
  second)
    _command_names
    ;;
  rest)
    shift words
    (( CURRENT -= 1 ))
    _normal
    ;;
esac
