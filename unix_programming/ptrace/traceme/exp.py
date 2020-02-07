import binascii

# this way cannot solve the problem...
print( binascii.a2b_hex('72505f617b4d5341')[::-1] + binascii.a2b_hex('61635f3535656330')[::-1] + binascii.a2b_hex('636172745f625f6e')[::-1] + binascii.a2b_hex('5f59316e305f6433')[::-1] + binascii.a2b_hex('0000007d33636e30')[::-1])
