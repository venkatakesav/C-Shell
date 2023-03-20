# The statement echo    is invalid (echo followed by just spaces is invalid) -> it is impossible to determine the number of spaces in this case
# Here if the shell recieves the command cd .. when in the home directory we just print the absolute path of the home directory -> It is impossible to go above the home directory as that would interfere with our notion of home
# Here the spaces and tabs are retained in echo (as in the original shell)
# Here all the commands that come before & are maintained in an array for further use are not executed right now.