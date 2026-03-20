#ifndef MACRO_H
# define MACRO_H

# define BBsq(file, rank) ((rank) * 8 + (file))
# define BBfile(sq) ((sq) & 7)
# define BBrank(sq) ((sq) >> 3)

#else

# undef BBsq
# undef BBfile
# undef BBrank

# undef MACRO_H

#endif /* #ifndef MACRO_H */
