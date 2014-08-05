# -*- coding: utf-8 -*-
sequence = """>1
YLYEIAR
>2
QTALVELLK
>3
LVNELTEFAK
>4
FKDLGEEHFK
>5
HPEYAVSVLLR
>6
HLVDEPQNLIK
>7
TVMENFVAFVDK
>8
RHPEYAVSVLLR
>9
LGEYGFQNALIVR
>10
DAFLGSFLYEYSR
>11
KVPQVSTPTLVEVSR
>12
HPYFYAPELLYYANK
>13
RHPYFYAPELLYYANK"""

from Bio.Blast import NCBIWWW
#result_handle = NCBIWWW.qblast("blastp", "nr",sequence)
#
#save_file = open("my_blast.xml", "w")
#save_file.write(result_handle.read())
#save_file.close()
#result_handle.close()

result_handle = open("my_blast.xml", "r")

from Bio.Blast import NCBIXML
blast_records = NCBIXML.parse(result_handle)
i = 1
#E_VALUE_THRESH = 0.04
for blast_record in blast_records:
    print("++++++++++ %d +++++++++++++" %i)
    for alignment in blast_record.alignments:
        for hsp in alignment.hsps:
            #if hsp.expect < E_VALUE_THRESH:
            print('****Alignment****')
            print('sequence:', alignment.title.split(">"))
            print('length:', alignment.length)
            print('score:', hsp.score)
            print('e value:', hsp.expect)
            print('num alignments:', hsp.align_length)
            print('identities:', hsp.identities)
            print('gaps:', hsp.gaps)
            print('positive:', hsp.positives)
            print(hsp.query + '...')
            print(hsp.match + '...')
            print(hsp.sbjct + '...')
    i+=1


