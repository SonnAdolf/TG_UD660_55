55��H3����

UART1  �����ն�  �����
����ͷ 9V034
ͨ�ŷ�ʽ������
H3Ϊʥ�㶨�ư���
55�����Ե��豸��660�ṹ��

��д���񣺣�����ԭ���h3ֻ��.img��������Ҫ������дemmc��
��д�ľ�����sd��emmcĿ¼
���裺
mount /dev/mmcblk0p3 /mnt/sd
yes | mkfs.ext4 /dev/mmcblk1
./write_emmc.sh /dev/mmcblk1
�����ϵ磬��չ
e2fsck -y -f /dev/mmcblk1p2
resize2fs /dev/mmcblk1p2




2017/11/2
