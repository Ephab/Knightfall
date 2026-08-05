1. Make move --done (except for castling and enpassent and so on)
2. Unmove --done (except for special moves)
3. make piece generation per piece then loop over the bitboard --done
4. Colors 
5. Legal moves not pseudolegal
6. castling isn't implemented 
7. en passant isn't implemented 
8. promotion is incomplete 
9. pinned pieces aren't handled
10. refactor code into helper files and into correct classes
11. I can probably use SIMD with looping to find the pieces in the bitboard