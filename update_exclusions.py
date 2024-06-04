import os

def read_exclude_conf(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
    return [line.strip() for line in lines if line.strip() and not line.startswith('#')]

#def update_gitignore(excludes):
#    lines = []
#    with open('.gitignore', 'r') as file:
#        lines = file.readlines()
#
#    with open('.gitignore', 'w') as file:
#        write_excludes = False
#        for line in lines:
#            if line.startswith('# Exclude specific files and directories'):
#                write_excludes = True
#            elif write_excludes and line.startswith('# End of excludes'):
#                write_excludes = False
#            if not write_excludes:
#                file.write(line)
#
#        file.write('\n# Exclude specific files and directories\n')
#        for exclude in excludes:
#            file.write(f'!{exclude}\n')
#        file.write('# End of excludes\n')

def update_innosetup_script(excludes):
    lines = []
    with open('setupCompiler.iss', 'r') as file:
        lines = file.readlines()

    with open('setupCompiler.iss', 'w') as file:
        for line in lines:
            if line.strip().startswith('Source: "Data\\*";'):
                new_line = line.split('Excludes:')[0] + f'Excludes: "{",".join(excludes)}";\n'
                file.write(new_line)
            else:
                file.write(line)

def update_build_gradle(excludes):
    lines = []
    path = 'Android/app/build.gradle.kts'
    with open(path, 'r') as file:
        lines = file.readlines()

    with open(path, 'w') as file:
        for line in lines:
            if line.strip().startswith('ignoreAssetsPattern = "'):
                new_line = line.split('ignoreAssetsPattern = "')[0] + f'ignoreAssetsPattern = "{":".join(excludes)}"\n'
                file.write(new_line)
            else:
                file.write(line)

def main():
    excludes = read_exclude_conf('data/.gitignore')
    #update_gitignore(excludes)
    update_innosetup_script(excludes)
    update_build_gradle(excludes)

if __name__ == "__main__":
    main()