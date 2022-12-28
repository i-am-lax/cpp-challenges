# Error-Correcting Code

## Description
An error-correcting code encodes a stream of binary digits (bits) for transmission over an unreliable channel such that (some) bit errors can not only be detected by the receiver, but can also be corrected.
Consider the steps in the transmission of the (very short single-letter) message "A" over an
unreliable channel from a sender to a receiver:

1 -

The sender encodes each character of the message as a sequence of 8 bits. In our case, the ASCII code for 'A' is 65. 65 is 01000001 in binary so the stream of data bits to be sent is 01000001.

2 -

The sender now inserts check bits (extra error correction information) into the stream of data bits. This is done by considering 4 data bits (call them `d1`, `d2`, `d3`, `d4`) at a time, and calculating 3 corresponding check bits (call them `c1`, `c2`, `c3`). The 4 bits of data and 3 check bits are then interleaved and transmitted as a 7-bit code word in the order: `c1`, `c2`, `d1`, `c3`, `d2`, `d3`, `d4`.

The check bits are calculated as follows:
```
c1 = parity(d1, d2, d4)
c2 = parity(d1, d3, d4)
c3 = parity(d2, d3, d4)
```

Here `parity(a, b, c)` is 0 if (a + b + c) is even, and 1 otherwise.

In our example, the first four bits of our data stream are 0100 (so `d1 = 0`, `d2 = 1`, `d3 = 0`, `d4 = 0`). The check digits are `c1 = 1`, `c2 = 0`, `c3 = 1`. Consequently the 7-bit code word transmitted is 1001100. The next four bits of our data stream are 0001, with corresponding 7-bit code word 1101001. Thus the error-corrected data stream transmitted is 1001100 1101001.

3 -

During transmission from sender to receiver, the error-corrected data stream might undergo some bit corruption. In our example, suppose bit 6 (currently a 0) becomes a 1, so that the received data stream is 1001110 1101001.

4 -

The receiver considers each 7-bit code word received in turn (call the bits received `b1`, `b2`, `b3`, `b4`, `b5`, `b6`, `b7`) and computes three parity check values (call them `p1`, `p2`, `p3`) as follows:
```
p1 = parity(b4, b5, b6, b7)
p2 = parity(b2, b3, b6, b7)
p3 = parity(b1, b3, b5, b7)
```
Here `parity(a, b, c, d)` is 0 if (a + b + c + d) is even, and 1 otherwise.

If `p1`, `p2` and `p3` are all 0, there were no single-bit errors in the transmission of this code word, and the original data bit stream can be recovered as `b3`, `b5`, `b6`, `b7`. If one or more of `p1`, `p2`, `p3` are 1, however, then there has been an error. The position of the bit error is given by the decimal value of the binary number `p1p2p3`. This bit can be flipped to correct the error, and the original data bit stream recovered as `b3`, `b5`, `b6`, `b7`.

In our example, the first 7-bit code word received is 1001110. The parity bits are `p1 = 1`, `p2 = 1`, `p3 = 0`. Thus the position of the bit error in binary is 110, which is 6 in decimal. Flipping bit 6 yields corrected code word 1001100, and extracting `b3`, `b5`, `b6`, `b7` gives corrected data bits 0100.

The next 7-bit code word received is 1101001. Now the parity bits are `p1 = 0`, `p2 = 0`, `p3 = 0`. This indicates that there were no single-bit errors. The original data bit stream can be recovered by extracting `b3`, `b5`, `b6`, `b7` to give data bits 0001.

5 -

The receiver has thus received the correct binary data stream (0100 0001) even though a bit error was made in transmission. (In fact the error correction is strong enough to correct a single-bit error in every 7-bit code word transmitted).

## Objective

### 1
Write a function `text_to_binary(str, binary)` which converts a string of characters (the input parameter `str`) into a corresponding stream of bits (the output string parameter `binary`). Also write a complementary function `binary_to_text(binary, str)` which converts a stream of bits (the input string parameter `binary`) into a corresponding character string (the output parameter `str`).
For example the code:
```
char binary[512];
text_to_binary("Art",binary);
```
should result in the string binary having the value `"010000010111001001110100"` (This is because the binary representations of the ASCII codes of 'A', 'r' and 't' are `01000001`, `01110010` and `01110100` respectively).

In a complementary fashion, the code:
```
char text[32];
binary_to_text("010000010111001001110100", text);
```
should result in the string text having the value `"Art"`.

### 2
Write a function add `error_correction(data, corrected)` which outputs (in the output string parameter `corrected`) an error-corrected version of a data bit stream (the input string parameter `data`).
For example, the code:
```
char correct[512];
add_error_correction("0100", correct);
```
should result in the string correct having the value `"1001100"`.

Similarly, the code:
```
char correct[512];
add_error_correction("01000001", correct);
```
should result in the string correct having the value `"10011001101001"`.

### 3
Write a function `decode(received, decoded)` which outputs (in the output string parameter `decoded`) an error-corrected version of a received bit stream (the input string parameter `received`). The function should return the number of errors corrected.
For example, the code:
```
char decoded[512];
errors = decode("1001110", decoded);
```
should result in the string decoded having the value `"0100"`, with the value of errors set to 1.