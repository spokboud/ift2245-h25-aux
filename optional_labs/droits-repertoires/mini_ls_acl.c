// https://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html
// https://www.ibm.com/docs/en/zos/2.4.0?topic=services-file-type-file-mode-values
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <time.h>
#include <unistd.h>
// S_ISUID This is the set-user-ID on execute bit, usually 04000
// S_ISGID This is the set-group-ID on execute bit, usually 02000.
// S_ISVTX This is the sticky bit, usually 01000.

// cat /etc/group
// man man
// man group.5
// afficher groupes: cat /etc/group | grep root
// changer password groupe: sudo gpasswd root

// afficher users: cat /etc/passwd
// changer le password dun user: sudo passwd root
/************************************************
 * Use StickyBit on directories
 *  chmod +t dir
 *  chmod 1777 dir
 *
 *  POUR VOIR LE STICKY BIT
 *  ls -ld mini_acl_FALSE_DIRECTORY_STICKYBIT
 ********************q****************************/
void mode_to_chars(mode_t mode, char* chars) {
    // changer le propriettaire du fichier: sudo chown root mini_ls_acl
    // set S_ISUID: sudo chmod 4744 mini_ls_acl_ROOTUID
    if (S_ISUID & mode)
        chars[0] = 's';
    // changer le groupe d'un fichier: sudo chgrp root mini_ls_acl_ROOTGID
    // set S_ISGID: sudo chmod 2075 mini_ls_acl_ROOTGID
    // ajouter user guib dans groupe guest: sudo usermod -a -G guest guib
    /*************************************
    *  Pour creer fichier ou repertoire avec un nouveau groupe
    *  -------------------------------------------------------
    *  sudo groupadd y
    *  sg y 'touch bar'
    *  # or
    *  newgrp y
    *  touch bar
    **************************************/
    else if (S_ISGID & mode)
        chars[0] = 'g';
    // sudo chmod 1744 mini_ls_acl_ROOTGID
    else if (S_ISVTX & mode)
        chars[0] = 'd';
    else
        chars[0] = '-';
    chars[1] = S_IRUSR & mode ? 'r' : '-';
    chars[2] = S_IWUSR & mode ? 'w' : '-';
    chars[3] = S_IXUSR & mode ? 'x' : '-';
    chars[4] = S_IRGRP & mode ? 'r' : '-';
    chars[5] = S_IWGRP & mode ? 'w' : '-';
    chars[6] = S_IXGRP & mode ? 'x' : '-';
    chars[7] = S_IROTH & mode ? 'r' : '-';
    chars[8] = S_IWOTH & mode ? 'w' : '-';
    chars[9] = S_IXOTH & mode ? 'x' : '-';
    chars[10] = 0;
}

int main(int argc, char* argv[]) {
    int begin = 1;
    if (2 > argc) {
        return 1;
    }

    struct stat file_stat;
    struct stat file_stat2;
    char mod_time[16];
    int st = 0;
    printf("Numéro inode  | Droits      | Nombre de liens  |    uid |    gid |    Taille | Dernière modification | \n");
    char perms[11];
    int color;
    for (int i = begin; i < argc; i++) {
        char* filename = argv[i];
        char* has_acl = "";
        st = lstat(filename, &file_stat);
        // access control lists
        // man lgetxattr
        // https://www.ibm.com/docs/en/spectrum-scale/4.2.3?topic=STXKQY_4.2.3/com.ibm.spectrum.scale.v4r23.doc/bl1hlp_accessfilesystemacl.html
        // https://www.thegeekdiary.com/how-to-configure-aclaccess-control-lists-in-linux-filesystem/
        if (lgetxattr(filename, "system.posix_acl_access", NULL, 0) != -1)
            // compile with option for acl files: -lacl
            has_acl = "+";
        if (st != 0) {
            perror(NULL);
        } else {
            mode_to_chars(file_stat.st_mode, perms);
            strftime(mod_time, 15, "%b %d %H:%M", localtime(&file_stat.st_mtim.tv_sec));
            // Est-ce que c'est un lien symbolique
            if (S_ISLNK(file_stat.st_mode)) {
                color = 96;
                char* pointed = malloc(file_stat.st_size + 1);
                pointed[file_stat.st_size] = 0; // Changer pour la vraie size
                if (stat(filename, &file_stat2) == -1) {
                    perror(NULL);
                    color = 31;
                }
                readlink(filename, pointed, file_stat.st_size);
                printf("%-13lu | %-7s%-1s | %-16lu | %-6u | %-6u | %-9lu | %-21s | \e[%dm %s -> %s \e[0m\n", file_stat.st_ino, perms, has_acl, file_stat.st_nlink, file_stat.st_uid, file_stat.st_gid, file_stat.st_size, mod_time, color, filename, pointed);
                free(pointed);
            } else {
                if (S_ISDIR(file_stat.st_mode)) {
                    color = 34;
                } else if (S_IXUSR & file_stat.st_mode) {
                    color = 32;
                }
                printf("%-13lu | %-7s%-1s | %-16lu | %-6u | %-6u | %-9lu | %-21s | \e[%dm %s \e[0m\n", file_stat.st_ino, perms, has_acl, file_stat.st_nlink, file_stat.st_uid, file_stat.st_gid, file_stat.st_size, mod_time, color, filename);
            }
        }
    }


    return 0;
}
