# -*- coding: utf-8 -*-
sequence = """>gi|1
YLYEIAR
>gi|2
QTALVELLK
>gi|3
LVNELTEFAK
>gi|4
FKDLGEEHFK
>gi|5
HPEYAVSVLLR
>gi|6
HLVDEPQNLIK
>gi|7
TVMENFVAFVDK
>gi|8
RHPEYAVSVLLR
>gi|9
LGEYGFQNALIVR
>gi|10
DAFLGSFLYEYSR
>gi|11
KVPQVSTPTLVEVSR
>gi|12
HPYFYAPELLYYANK
>gi|13
RHPYFYAPELLYYANK"""

from Bio.Blast import NCBIWWW
result_handle = NCBIWWW.qblast("blastp", "nt",sequence)

#save_file = open("my_blast.xml", "w")
#save_file.write(result_handle.read())
#save_file.close()
#result_handle.close()

from Bio.Blast import NCBIXML
blast_records = NCBIXML.parse(result_handle)
i = 1
E_VALUE_THRESH = 0.04
for blast_record in blast_records:
    print("++++++++++ %d +++++++++++++" %i)
    for alignment in blast_record.alignments:
        for hsp in alignment.hsps:
            if hsp.expect < E_VALUE_THRESH:
                print('****Alignment****')
                print('sequence:', alignment.title)
                print('length:', alignment.length)
                print('e value:', hsp.expect)
                print(hsp.query[0:75] + '...')
                print(hsp.match[0:75] + '...')
                print(hsp.sbjct[0:75] + '...')
    i+=1


