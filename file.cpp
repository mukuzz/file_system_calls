#include <iostream>
#include <fcntl.h> // creat()
#include <sys/stat.h> // fstat()
#include <unistd.h> // lseek(), read(), write()

void create_norm_file(const char *path, int mode) {
    // create a new file with given path or rewrite if it already exists
    if (creat(path, mode) == -1) {
        perror("create");
        exit(EXIT_FAILURE);
    } else {
        printf("create: regular file created with path %s and ", path);
        printf("mode: %o (octal)\n",  S_IFREG | mode);
        exit(EXIT_SUCCESS);
    }
}

void create_pipe(const char *path, int mode) {
    int modeAndType = S_IFIFO | mode;
    if (mknod(path, modeAndType, 0) == -1) {
        perror("create");
        exit(EXIT_FAILURE);
    } else {
        printf("create: pipe created with path %s and ", path);
        printf("mode: %o (octal)\n",  modeAndType);
        exit(EXIT_SUCCESS);
    }
}

void read_file(const char *path, int offset, int bytes) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // seek file if offset is set
    if (offset > 0) {
        if (lseek(fd, offset, SEEK_SET) == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }
    }

    int b_size = 1024;
    char buf[b_size];
    int read_count = 0, total_read = 0;
    while(true) {
        // if fixed number of bytes are to be read
        if (bytes >= 0) {
            // terminate if required bytes read
            if (total_read >= bytes) exit(EXIT_SUCCESS);
            // if next read will exceed required bytes, reduce read size.
            if (total_read + b_size > bytes)
                b_size = bytes - total_read;
        }
        // read one less byte to be able to add \0(null char) later
        read_count = read(fd, &buf, b_size - 1);
        if (read_count == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (read_count == 0) {
            exit(EXIT_SUCCESS);
        }
        // add null character at end for correct output
        buf[read_count] = '\0';
        std::cout << buf;
        
        total_read += read_count;
    };
    
    if (close(fd) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
}

void write_file(const char *path, int offset, int bytes) {
    int fd_write = open(path, O_WRONLY);
    if (fd_write == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // seek file if offset is set
    if (offset > 0) {
        if (lseek(fd_write, offset, SEEK_SET) == -1){
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    int b_size = 1024;
    char buf[b_size];
    int read_count = 0, write_count = 0, total_write = 0;
    while(true) {
        // if fixed number of bytes are to be written
        if (bytes >= 0) {
            // terminate if required bytes written
            if (total_write >= bytes) exit(EXIT_SUCCESS);
            // if next write will exceed required bytes, reduce write size.
            if (total_write + b_size > bytes)
                b_size = bytes - total_write;
        }

        // read from stdin
        read_count = read(0, &buf, b_size);
        if (read_count == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        if (read_count == 0) {
            exit(EXIT_SUCCESS);
        }

        // write to file
        write_count = write(fd_write, &buf, read_count);
        if (write_count == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        
        total_write += write_count;
    };
    
    if (close(fd_write) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
}

void file_stat(const char *path) {
    struct stat sb;
    if (stat(path, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    // bitwise AND with S_IFMT bitmask to consider only bits involved with filetype
    switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:  printf("block device\n");            break;
    case S_IFCHR:  printf("character device\n");        break;
    case S_IFDIR:  printf("directory\n");               break;
    case S_IFIFO:  printf("FIFO/pipe\n");               break;
    case S_IFLNK:  printf("symlink\n");                 break;
    case S_IFREG:  printf("regular file\n");            break;
    case S_IFSOCK: printf("socket\n");                  break;
    default:       printf("unknown?\n");                break;
    }

    printf("I-node number:            %ld\n", (long) sb.st_ino);
    printf("Mode:                     %o (octal)\n",  sb.st_mode);
    printf("Link count:               %ld\n", sb.st_nlink);
    printf("Ownership:                UID=%ld   GID=%ld\n", (long) sb.st_uid, (long) sb.st_gid);
    printf("File size:                %lld bytes\n", (long long) sb.st_size);
    printf("Preferred I/O block size: %ld bytes\n", (long) sb.st_blksize);
    printf("Blocks allocated:         %lld\n", (long long) sb.st_blocks);
    printf("Last status change:       %s", ctime(&sb.st_ctime));
    printf("Last file access:         %s", ctime(&sb.st_atime));
    printf("Last file modification:   %s", ctime(&sb.st_mtime));

    exit(EXIT_SUCCESS);
}