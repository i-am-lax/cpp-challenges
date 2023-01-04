# Digital Stamp

## Description
Spam email is a pervasive nuisance that accounts for around 60% of global email traffic. This task is about implementing a scheme that is designed to tackle this problem. The scheme works by making senders "pay" for every email they send by having to invest computational effort in the creation of a "digital stamp" added to email headers. The validity of the stamp can be verified by a receiver at negligible computational cost. The idea is to transfer the cost of spam from receivers to senders, thus rendering the business models of spam email senders unprofitable.

Our solution will make use of _hash functions_. A hash function is a function which maps data of arbitrary length onto a fixed size fingerprint known as a digest. The hash function we will use is called **SHA1**. The digest for this function is 20 bytes long and is most conveniently displayed in human-readable form as 40 hexadecimal digits.

For example, the SHA1 digest of the message:
```
The bitcoin is under the carpet in the living room.
```
is `60eb8db1a3c0ea1ad3d5548f248b4a3e73711ee5`.

Hash functions are designed such that changing the input, even slightly, will result in a very different digest. For example, the SHA1 digest of the message:
```
The bitcoin is under the carpet in the dining room.
```
is `29c22640a6df69a7fcb9d8d4923b90f817d36195`.

The process for sending an email works as follows:
* The sender computes the SHA1 digest of the message body they wish to send
* The sender prepares a header of the following format: `<recipient email address>:<SHA1 digest of message body>:<counter>` where counter is an integer initalised to 0. For example: `wjk@imperial.ac.uk:60eb8db1a3c0ea1ad3d5548f248b4a3e73711ee5:0`
* The sender then computes the SHA1 digest of the header. If the first 5 hex digits (the first 20 bits) of the digest are zero, then this is an acceptable header. If not, the sender increments the counter and tries the hash again. In the example above, the first counter value that gives an acceptable header is 313889, since the SHA1 digest of the header `wjk@imperial.ac.uk:60eb8db1a3c0ea1ad3d5548f248b4a3e73711ee5:313889` is `00000e269e4da81b701ed39646eafe17ae9aadd0`

To validate a received email the receiver must:
* Confirm that the header is in the correct format (three fields separated by `:`)
* Check the recipient mentioned in the first field of the header matches the expected recipient
* Check that the SHA1 digest of the message body given in the header matches the actual SHA1 digest of the received message
* Check that the SHA1 digest of the header is an acceptable header

If all of these conditions are met, the email is valid (and can be accepted). Otherwise the email is invalid (and should be rejected).

Supplied files:
* `message1.txt`, `message2.txt` and `message3.txt` - contains sample messages

You can verify what the corresponding SHA1 digests of these files are using the sha1sum utility:
```
% sha1sum message?.txt
60eb8db1a3c0ea1ad3d5548f248b4a3e73711ee5 message1.txt
29c22640a6df69a7fcb9d8d4923b90f817d36195 message2.txt
fe8e2cc69298b811f53dcfd54929d02cc65748e3 message3.txt
```

## Objective

### 1
Write an integer-valued function `leading_zeros(digest)` which takes as its parameter a hexadecimal string representing a hash digest and returns the number of leading zeros it begins with. If `digest` contains characters that are not valid hexadecimal digits ('0' to '9' and 'a' to 'f') then the function should return `-1`.
For example, the code:
```
cout << "The number of leading zeros in digest "
  << "’00000a184d72f39730312554e0df25e6f95a05d4’ is "
  << leading_zeros("00000a184d72f39730312554e0df25e6f95a05d4");
```
should display the output
```
The number of leading zeros in digest ’00000a184d72f39730312554e0df25e6f95a05d4’ is 5
```

### 2
Write a boolean function `file_to_SHA1_digest(filename, digest)` which returns in `digest` the SHA1 digest of the contents of the file with name `filename`. If the file cannot be opened then the function should return `false` with digest set to `"error"`; otherwise return `true`.
For example, the code:
```
char digest[41];
bool success = file_to_SHA1_digest("message1.txt", digest);
cout << "SHA1 digest of file ’message1.txt’ is " << digest << endl;
if (!success)
  cout << "There was an error reading the file." << endl;
```
should display the output
```
SHA1 digest of file ’message1.txt’ is 60eb8db1a3c0ea1ad3d5548f248b4a3e73711ee5
```

### 3
Write a Boolean function `make_header(recipient, filename, header)` which prepares a header for a given email message. Here `recipient` is a read-only input parameter specifying the email address of the message recipient, filename is a read-only input parameter specifying the file containing the message body, and `header` is an output parameter containing the header in form:
```
<recipient email address>:<SHA1 digest of message body>:<counter>
```
The return value of the function should be `false` if the file containing the message body cannot be read, or if a valid counter is not found after 10 million attempts; otherwise the function should return `true`.
For example, the code:
```
char header[512];
bool success = make_header("wjk@imperial.ac.uk", "message1.txt", header);
if (success)
  cout << "A valid header for ’message1.txt’ is "<< header << endl;
else
  cout << "There was an error generating the header." << endl;
```
should result in the output
```
A valid header for ’message1.txt’ is
wjk@imperial.ac.uk:60eb8db1a3c0ea1ad3d5548f248b4a3e73711ee5:313889
```

### 4
Write a function `check_header(email_address, header, filename)` which features three input parameters: the user’s email address, a received message header and the name of a file containing the text of a received email body. The return value should of enumerated type `MessageStatus` (see `stamp.h`), which can be printed in a human-readable manner using the `message_status()` internal helper function in `main.cpp`.
For example, the code:
```
cout << "Checking email with header " << endl
  << "’wjk@imperial.ac.uk:60eb8db1a3c0ea1ad3d5548f248b4a3e73711ee5:313889’"
  << endl << "sent to ’wjk@imperial.ac.uk’ with body ’message1.txt’:" << endl;
MessageStatus result = check_header("wjk@imperial.ac.uk",
  "wjk@imperial.ac.uk:60eb8db1a3c0ea1ad3d5548f248b4a3e73711ee5:313889",
  "message1.txt");
cout << message_status(result) << endl;
```
should result in the following output:
```
Checking email with header
’wjk@imperial.ac.uk:60eb8db1a3c0ea1ad3d5548f248b4a3e73711ee5:313889’
sent to ’wjk@imperial.ac.uk’ with body ’message1.txt’:
Valid message.
```