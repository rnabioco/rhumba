import binascii
import collections

def is_gz_file(filepath):
    with open(filepath, 'rb') as test_f:
        return binascii.hexlify(test_f.read(2)) == b'1f8b'

def base_counter():
    """create counter object populated with canonical bases"""

    bases_to_count = ["A","T","C","G","N"]
    base_dict = collections.Counter()

    for base in bases_to_count:
        base_dict[base] = 0

    return base_dict

