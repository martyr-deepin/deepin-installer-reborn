<a name="2.7.15"></a>
### 2.7.15 (2019-07-04)


#### Others

*   add language for lithuania serbian catalan ([5922b867](https://github.com/linuxdeepin/deepin-install-reborn/commit/5922b867aa2efcb1707c43cdbc65856513785c01))
*   change report server url ([c3ac01b8](https://github.com/linuxdeepin/deepin-install-reborn/commit/c3ac01b8b7ae18c39c2e7d32cac2b6149b51791e))



<a name="2.7.14"></a>
### 2.7.14 (2019-06-25)


#### Others

*   disable disk crypt for loongson ([0ea3c1c9](https://github.com/linuxdeepin/deepin-install-reborn/commit/0ea3c1c97ba6f99b14091acb8058342346343438))
* **longxin:**  remove grub link file ([44652f07](https://github.com/linuxdeepin/deepin-install-reborn/commit/44652f073d199a757f02b3ec9832bf7120a21642))
* **loongson:**  update 49_copy_boot_files.job ([197567f1](https://github.com/linuxdeepin/deepin-install-reborn/commit/197567f193be5cdc7bf2efab0d469070241e880c))
* **partition:**  gpt table need 33 sectors in the end ([91c3ab52](https://github.com/linuxdeepin/deepin-install-reborn/commit/91c3ab528a4a76e6ba6693fc3d18c0c624247985))



<a name="2.7.13"></a>
### 2.7.13 (2019-06-13)


#### Others

*   update apt repository settings for all platform ([048efe51](https://github.com/linuxdeepin/deepin-install-reborn/commit/048efe51fedb95cb5016e8142b06c8a79490686d))



<a name="2.7.12"></a>
### 2.7.12 (2019-06-13)


#### Others

*   update all platform settings ([67f6e457](https://github.com/linuxdeepin/deepin-install-reborn/commit/67f6e4573a5a27392db5a00e8ed28f98c14efbcb))



<a name=""></a>
##  2.7.11 (2019-06-11)


#### Features

* **endpoint:**
  *  display ip and mac info ([d3581cb3](https://github.com/linuxdeepin/deepin-install-reborn/commit/d3581cb3462201abae10cda3059567ee102e9a39))
  *  add server url for oem ([6cbee4c7](https://github.com/linuxdeepin/deepin-install-reborn/commit/6cbee4c72a9f65cd3732bcd95a0102a6d09fca69))



<a name="2.7.10"></a>
### 2.7.10 (2019-06-11)




<a name="2.7.9"></a>
### 2.7.9 (2019-06-11)


#### Features

*   analyze the time of scripts ([5f998ad4](https://github.com/linuxdeepin/deepin-install-reborn/commit/5f998ad41a0a0d278bb65687e2befb20140bc752))
*   add run hook script time ([b6e71731](https://github.com/linuxdeepin/deepin-install-reborn/commit/b6e717313c061fe08a88251fdcd1a7c94a0050de))

#### Bug Fixes

*   fix: first boot crash #63 (#72) ([985fa4d4](https://github.com/linuxdeepin/deepin-install-reborn/commit/985fa4d4a3337affe2333d5cfd4a8609524704cf))



<a name="2.7.8"></a>
### 2.7.8 (2019-06-05)


#### Bug Fixes

* **partition:**  empty device object ([968c99ec](https://github.com/linuxdeepin/deepin-install-reborn/commit/968c99ecffd97fcc041a45e64d3b52d583993a90))



<a name="2.7.7"></a>
### 2.7.7 (2019-06-04)


#### Bug Fixes

*   first boot crash (#63) ([1f2e3972](https://github.com/linuxdeepin/deepin-install-reborn/commit/1f2e3972d7e21be6b0eb045eb3c445b9db18a140))
* **longxin:**  full disk crypt cannot input password ([e635097d](https://github.com/linuxdeepin/deepin-install-reborn/commit/e635097d978b0f3ffd4a9117f9fd40015c312bf8))

#### Features

* **Disk:**  add dynamic disk warning page ([a87206a0](https://github.com/linuxdeepin/deepin-install-reborn/commit/a87206a093cf87302d5125ea6f7ec8a486531f90))



<a name="2.7.6"></a>
### 2.7.6 (2019-05-31)


#### Bug Fixes

* **checkMode:**
  *  missing DI_SI_USER ([f65b7db6](https://github.com/linuxdeepin/deepin-install-reborn/commit/f65b7db6782888c2406eecdc5ba50168adedfa74))
  *  change file order ([d8533b0e](https://github.com/linuxdeepin/deepin-install-reborn/commit/d8533b0ef963c6e7621ce5184368f1cc3bdf5310))



<a name="2.7.5"></a>
### 2.7.5 (2019-05-31)


#### Bug Fixes

*   disable osprober on sw and longxin ([4f4df470](https://github.com/linuxdeepin/deepin-install-reborn/commit/4f4df470993fe790107d4204b00ddff0b6cd6bbc))
* **autoScale:**  not check outputInfo nullptr (#59) ([882446be](https://github.com/linuxdeepin/deepin-install-reborn/commit/882446be2fa9d8c936422ee6b73c37d692cdd16b))
* **checkMode:**  missing get DI_SI_USER value ([b66fc972](https://github.com/linuxdeepin/deepin-install-reborn/commit/b66fc9720a14039267a61e062bd11a5ab3d95c0b))
* **oem:**
  *  grub background invalid ([7fd332ff](https://github.com/linuxdeepin/deepin-install-reborn/commit/7fd332ff6255140af6c2c3a24679f9e177b3062d))
  *  greeter invalid ([04a7ddee](https://github.com/linuxdeepin/deepin-install-reborn/commit/04a7ddee87f1751ce1130ead7cc550d2f16a5890))



<a name="2.7.4"></a>
### 2.7.4 (2019-05-27)


#### Features

*   support skip language page on first boot ([80a4e108](https://github.com/linuxdeepin/deepin-install-reborn/commit/80a4e1082797bc3e969d3c8988dc7878678b29e1))
*   add check system mode ([b240bd6b](https://github.com/linuxdeepin/deepin-install-reborn/commit/b240bd6b2268c9c6d3bc9121d4c1a32abf12c6df))

#### Bug Fixes

*   font in first boot setup pages seems strange ([a7910706](https://github.com/linuxdeepin/deepin-install-reborn/commit/a79107067dbdb60e76701cc5976d18fab971be8a))



<a name="2.7.3"></a>
### 2.7.3 (2019-05-24)


#### Bug Fixes

*   No language configuration read after startup ([f2148d82](https://github.com/linuxdeepin/deepin-install-reborn/commit/f2148d8244842c3c7c516d3313ecf219328671d7))



<a name="2.7.2"></a>
### 2.7.2 (2019-05-16)


#### Features

*   add control platform title and subtitle ([e77126b0](https://github.com/linuxdeepin/deepin-install-reborn/commit/e77126b0a0a952b53e00741a3afad4d726737c04))
*   add control platform page ([8d69e212](https://github.com/linuxdeepin/deepin-install-reborn/commit/8d69e212e9b220d00ba93bfb5d9387eb0c27db08))
*   support oem customization of sources.list (#38) ([b5c38d5c](https://github.com/linuxdeepin/deepin-install-reborn/commit/b5c38d5c530bc546c658f37d528cab8f845eeea0))
* **ControlPlatform:**  add base ui ([ee9745cc](https://github.com/linuxdeepin/deepin-install-reborn/commit/ee9745ccbe1616d9e540c5efc573bebe0ba1f382))



<a name="2.7.1"></a>
### 2.7.1 (2019-05-15)


#### Features

*   save network config ([58e0cece](https://github.com/linuxdeepin/deepin-install-reborn/commit/58e0cece8666737bb18e9c8c6e02951b3ecd57b2))
*   add network valid check ([8d4c54e9](https://github.com/linuxdeepin/deepin-install-reborn/commit/8d4c54e9b0ac332b112b82a93c373563d83cdb5c))
*   add network settings page ([6823263b](https://github.com/linuxdeepin/deepin-install-reborn/commit/6823263b4856336776665233d77ba4c5ca9a287e))
* **Network:**
  *  add skip_network_page to default_settings.ini ([edecbaa1](https://github.com/linuxdeepin/deepin-install-reborn/commit/edecbaa1ca70322143f76aa8aa2658125fc4dc90))
  *  add skip button to next page ([c00e1834](https://github.com/linuxdeepin/deepin-install-reborn/commit/c00e18345ce75abc8f359b54c170acb05dd3475a))
  *  add edit placeholderText ([d260c78c](https://github.com/linuxdeepin/deepin-install-reborn/commit/d260c78cddd3241feabf1800f33b86d9fda597a8))
* **network:**  add logo and subtitle ([99e849b6](https://github.com/linuxdeepin/deepin-install-reborn/commit/99e849b686de830bce6e7ddfa76f9819240ca4c8))



<a name=""></a>
##  2.7.0 (2019-04-24)


#### Bug Fixes

* **avatar:**  array out of bound ([6cacf2a9](https://github.com/linuxdeepin/deepin-install-reborn/commit/6cacf2a9ed59d51f4b552d21f5d5ba8f2a07e0df))



<a name="2.6.11"></a>
### 2.6.11 (2019-04-22)


#### Bug Fixes

* **loogson:**  missing UUID ([e6cff53d](https://github.com/linuxdeepin/deepin-install-reborn/commit/e6cff53d881b5c2dfdcff351582a91cd2c7be494))



<a name="2.6.10"></a>
### 2.6.10 (2019-04-22)


#### Bug Fixes

*   not remove user password for conf file ([7be9fbd6](https://github.com/linuxdeepin/deepin-install-reborn/commit/7be9fbd615f7a2b25e48b32cf89884097fc584ab))
* **simple:**  install failed ([484c39be](https://github.com/linuxdeepin/deepin-install-reborn/commit/484c39be62b964efe5e597cea207fba664344439))
* **slide:**   slide oem dir path error ([966e164e](https://github.com/linuxdeepin/deepin-install-reborn/commit/966e164e8b2fd744245dd0b99d366bd91ca2af3c))



<a name="2.6.8"></a>
### 2.6.8 (2019-04-09)




<a name="2.6.7"></a>
### 2.6.7 (2019-04-02)


#### Bug Fixes

*   live-filesystem don't have password ([3c85aa6f](https://github.com/linuxdeepin/deepin-install-reborn/commit/3c85aa6f520b047378531f73767bb086768c1559))
*   disable plymouth when use full disk encryption on the longxin ([f5ae51f6](https://github.com/linuxdeepin/deepin-install-reborn/commit/f5ae51f6a5d22bf5397d448e674208b5a73b3f5f))
* **longxin:**  full crypt disk install failed ([39b7a2d0](https://github.com/linuxdeepin/deepin-install-reborn/commit/39b7a2d02a259c3bb733a9e3e860a988f8705d9a))
* **os-prober:**  pre-operation check ([0fd24459](https://github.com/linuxdeepin/deepin-install-reborn/commit/0fd2445949c219badfffef4dbb3ee946c81d6345))

#### Features

*   show only support filesystem ([800ca813](https://github.com/linuxdeepin/deepin-install-reborn/commit/800ca813aa7dd9cd84c5521753e983a23a2b5498))
*   update repo url ([f1e4c817](https://github.com/linuxdeepin/deepin-install-reborn/commit/f1e4c817cf3d03de0395ac14e88405ab4aa26f4e))
*   enable language scrollbar ([4366152b](https://github.com/linuxdeepin/deepin-install-reborn/commit/4366152b7297655dd3c18348b75aac351ca15890))
*   use new user license page ([41a641cb](https://github.com/linuxdeepin/deepin-install-reborn/commit/41a641cb6d20c10f012886ff4836a49631676235))
* **fulldisk:**  limit swap to 16G ([d031bc6e](https://github.com/linuxdeepin/deepin-install-reborn/commit/d031bc6e09676ead7bdb14deff17e06263b9112b))



<a name="2.6.6"></a>
### 2.6.6 (2019-03-06)


#### Bug Fixes

*   the actual partition size is incorrect ([cacbc46f](https://github.com/linuxdeepin/deepin-install-reborn/commit/cacbc46fe9ef699ebe6f526302ecf5bb36829e46))



<a name="2.6.5"></a>
### 2.6.5 (2019-03-04)


#### Features

*   exclude no need files for professional ([21488371](https://github.com/linuxdeepin/deepin-install-reborn/commit/2148837116f90251eb3a26410e396c4f4beaff0a))

#### Bug Fixes

* **FullDisk:**  cannot start loongson and sw when use full disk install ([fb0ee0d1](https://github.com/linuxdeepin/deepin-install-reborn/commit/fb0ee0d10831c20ce08f184a89ecb112ecf9366d))
* **Partition:**  missing check root partition ([a0cf1aab](https://github.com/linuxdeepin/deepin-install-reborn/commit/a0cf1aab3a100572a526d01f541a5cd8d7b81c4f))



<a name="2.6.4"></a>
### 2.6.4 (2019-01-30)




<a name="2.6.3"></a>
### 2.6.3 (2019-01-25)


#### Bug Fixes

*   failure when losing symbols ([6f1655d3](https://github.com/linuxdeepin/deepin-install-reborn/commit/6f1655d35b323ce3bc61ccaa0f280b6a6e300397))
* **Advance:**  partition starting number is not 1 ([431d4682](https://github.com/linuxdeepin/deepin-install-reborn/commit/431d4682a0dcbe6de2fe30fc3484aee974f7438e))



<a name="2.6.2"></a>
### 2.6.2 (2019-01-14)


#### Bug Fixes

*   missing format operation check ([d44a5697](https://github.com/linuxdeepin/deepin-install-reborn/commit/d44a5697a27266a0eaac864bdb50e76132994f4c))



<a name="2.6.1"></a>
### 2.6.1 (2019-01-11)


#### Features

*   use ext4&acl to replace ntfs as data storage ([19af31af](https://github.com/linuxdeepin/deepin-install-reborn/commit/19af31af92e2777af610381a07a3a01e7b6a6b51))
*   update hook for loongson ([7bccd5e4](https://github.com/linuxdeepin/deepin-install-reborn/commit/7bccd5e468b0d5284daece2970882d5608e1c7b3))
* **Simple:**  give priority to the current device efi partition ([2c32b936](https://github.com/linuxdeepin/deepin-install-reborn/commit/2c32b9366000678c0cb5390d74a16eba0f6e78b7))

#### Bug Fixes

*   scale ratio always 4x on vmware ([6074cb46](https://github.com/linuxdeepin/deepin-install-reborn/commit/6074cb461faaf7a7af36980f50b4b47da0d47784))
* **Edit:**  crash on editing parition details ([cfb485c4](https://github.com/linuxdeepin/deepin-install-reborn/commit/cfb485c4808f8c5f6bfb2ac9a041fb19ea7bfddd))



<a name=""></a>
##  2.6.0 (2019-01-10)


#### Features

*   update sw and loongson full disk config file ([e14bbac1](https://github.com/linuxdeepin/deepin-install-reborn/commit/e14bbac117d29811e4f2a51e36a35b814e1b85ac))
* **FullDisk:**  full disk encrypt support uppercase prompts ([dd687fb1](https://github.com/linuxdeepin/deepin-install-reborn/commit/dd687fb10cefe90927694cbeb478d5444175c619))

#### Bug Fixes

*   can't display all ([763acf26](https://github.com/linuxdeepin/deepin-install-reborn/commit/763acf2601654204c7b3f206316b4e2a007bbfbc))
*   windows may not be detected ([d449f62b](https://github.com/linuxdeepin/deepin-install-reborn/commit/d449f62bdd14040b2a4e7dbaa5a5917dee4d0a5b))
*   paritioning failed on simple mode with UEFI ([baf065cf](https://github.com/linuxdeepin/deepin-install-reborn/commit/baf065cff1927e4750b82b4c4d8789909bcf1597))
*   time standard may wrongly set to local time ([75f5ee33](https://github.com/linuxdeepin/deepin-install-reborn/commit/75f5ee33517e011f136bf23c5de315f525320c18))
* **Advanced:**  not check new table operation ([dec1bab3](https://github.com/linuxdeepin/deepin-install-reborn/commit/dec1bab36efe864c9f3a8877c528d6644d55b2b1))
* **Edit:**  null pointer exception ([e250d29b](https://github.com/linuxdeepin/deepin-install-reborn/commit/e250d29b178a840da2707789e0990af9903ed2e5))
* **FullDisk:**
  *  disable QCheckBox focus ([5e5858d0](https://github.com/linuxdeepin/deepin-install-reborn/commit/5e5858d0dfa6c3a0482c46215157aadb4db0d3eb))
  *  not set disk label for crypt mode ([7af20973](https://github.com/linuxdeepin/deepin-install-reborn/commit/7af209734e05632400e3472c5e619e6f17cc37f0))
  *  policy not updated ([ab298756](https://github.com/linuxdeepin/deepin-install-reborn/commit/ab2987567d0448fe17deea46b344ca3f8b159e37))



<a name="2.5.10"></a>
### 2.5.10 (2019-01-04)


#### Bug Fixes

*   full disk installation always fail ([6804e612](https://github.com/linuxdeepin/deepin-install-reborn/commit/6804e6127306a8fba90132050b891bd831dc7dca))



<a name="2.5.9"></a>
### 2.5.9 (2019-01-03)


#### Bug Fixes

*   device partitions info error ([514387ec](https://github.com/linuxdeepin/deepin-install-reborn/commit/514387ec463e734076e4824b3fedbc1bfb5e8b4c))



<a name="2.5.8"></a>
### 2.5.8 (2018-12-29)


#### Bug Fixes

*   UEFI cannot scan other system ([b6e49967](https://github.com/linuxdeepin/deepin-install-reborn/commit/b6e49967dd05d98e941e6c1b2630c2dc07cb17b3))
* **Partition:**  advanced partition not check create part tab ([5cf0111a](https://github.com/linuxdeepin/deepin-install-reborn/commit/5cf0111a41fd570523212ad776c47484095e809f))



<a name="2.5.7"></a>
### 2.5.7 (2018-12-25)


#### Features

*   wait 1s ([caf8b5cf](https://github.com/linuxdeepin/deepin-install-reborn/commit/caf8b5cf00e1d4128e638f17059d23d31d858a7c))
*   disable strong password check ([cd36ab78](https://github.com/linuxdeepin/deepin-install-reborn/commit/cd36ab78587e051b1cb3ff1db611fd44b888ccd4))
* **format:**  add two format attempts ([01c1a03c](https://github.com/linuxdeepin/deepin-install-reborn/commit/01c1a03ce4033e8be9961493781fa12f14867da8))

#### Bug Fixes

*   advanced partiton has failed data ([4c88e362](https://github.com/linuxdeepin/deepin-install-reborn/commit/4c88e3626b1bb388766cd49bb5ed0e03d340608a))



<a name="2.5.4"></a>
### 2.5.4 (2018-12-20)

*  update sources.list
*  rsync transifex  
*  feat grub passwd 


### 2.5.3 (2018-12-20)


#### Features

*   add format error output ([1fb77c68](https://github.com/linuxdeepin/deepin-install-reborn/commit/1fb77c688b8d320ab5bb97bcef167d4339b155e4))
*   update grub edit ([d50fd0e1](https://github.com/linuxdeepin/deepin-install-reborn/commit/d50fd0e1c613686b6cd18d42a18649cc4482050a))



<a name="2.5.2"></a>
### 2.5.2 (2018-12-18)


#### Bug Fixes

*   check grub script ([39087e1d](https://github.com/linuxdeepin/deepin-install-reborn/commit/39087e1d91ecf783beee404f6df6b05573fe39da))



<a name="2.5.1"></a>
### 2.5.1 (2018-12-18)


#### Bug Fixes

* **Grub:**
  *  not exec grub script ([ca0738c9](https://github.com/linuxdeepin/deepin-install-reborn/commit/ca0738c9d4fb90122b47fcdbe95af3e173341ce7))
  *  script error ([0999d8aa](https://github.com/linuxdeepin/deepin-install-reborn/commit/0999d8aa22aaf8285110fd673f28483c4458c825))



<a name=""></a>
##  2.5.0 (2018-12-17)


#### Bug Fixes

* **FullPartition:**  disable checkbox background ([d8c8058a](https://github.com/linuxdeepin/deepin-install-reborn/commit/d8c8058a938408bc49903c27fb412d572a44dce4))

#### Features

*   add grub edit password checkbox ([8ed5920f](https://github.com/linuxdeepin/deepin-install-reborn/commit/8ed5920fbbf1bb713b36942e449069b89a2279e5))
*   bind mount chroot needed ([49d1d9a1](https://github.com/linuxdeepin/deepin-install-reborn/commit/49d1d9a1a36cd46bdcaf3284c9926ad68cba2e63))
*   add touch scroll for user agreement frame ([57c147bb](https://github.com/linuxdeepin/deepin-install-reborn/commit/57c147bbe590358ab6b2e46e0f858c4d8a092680))
*   add user license switch language ([dff465f1](https://github.com/linuxdeepin/deepin-install-reborn/commit/dff465f157c839a7d6caf6bd09142c86685fd0b5))
*   give priority to the current device efi partition ([b5f664a7](https://github.com/linuxdeepin/deepin-install-reborn/commit/b5f664a70913e1f77d516c1da7e219e2c0abf8fb))
*   add password strong check ([fc0e85b9](https://github.com/linuxdeepin/deepin-install-reborn/commit/fc0e85b995233432f30c0d378f350d96dc9a08e1))
* **Grub:**  set edit password ([9570237c](https://github.com/linuxdeepin/deepin-install-reborn/commit/9570237cde6898971da8dba0eb76a34b386ae710))
* **SystemInfo:**  add capslock icon ([7fe4c7db](https://github.com/linuxdeepin/deepin-install-reborn/commit/7fe4c7db3a7b7d862f0d6d44209aee4a7ac7e848))
* **grub:**  set grub edit password ([4f15445c](https://github.com/linuxdeepin/deepin-install-reborn/commit/4f15445c01d38471c2249f6103eeb996c0ae4cc8))



<a name="2.4.9"></a>
### 2.4.9 (2018-11-13)


#### Bug Fixes

*   not load default translator ([b8cd2528](https://github.com/linuxdeepin/deepin-install-reborn/commit/b8cd252812e80820e38e093bb5c9987d3a513bc1))
*   not load translator ([e8fda02c](https://github.com/linuxdeepin/deepin-install-reborn/commit/e8fda02ce6535e04214660c0d6d1b4ae3e3167e1))
* **FullDisk:**  not set width ([7192f7d5](https://github.com/linuxdeepin/deepin-install-reborn/commit/7192f7d56ca5d045ac03851e94f162cc7044160b))



<a name="2.4.8"></a>
### 2.4.8 (2018-11-12)




<a name="2.4.7"></a>
### 2.4.7 (2018-11-12)


#### Bug Fixes

*   debug crash ([542ece20](https://github.com/linuxdeepin/deepin-install-reborn/commit/542ece207e369e51e4aecaca424bcdfd9ae0a75c))
*   wrong locale variable ([aa0dbace](https://github.com/linuxdeepin/deepin-install-reborn/commit/aa0dbaceb88ff3267a37fd76d90879912a425ac0))
* **FullDisk:**  without choose hard disk that can next step ([be52be30](https://github.com/linuxdeepin/deepin-install-reborn/commit/be52be30917a38266d138e8f36ad03c0d6476071))



<a name="2.4.6"></a>
### 2.4.6 (2018-11-09)




<a name="2.4.5"></a>
### 2.4.5 (2018-11-09)


#### Features

*   set grub theme language ([45d9941b](https://github.com/linuxdeepin/deepin-install-reborn/commit/45d9941b3a083f88b4df9264b9529c66ee9ecbfc))

#### Bug Fixes

*   remove useless grub config ([1f767b68](https://github.com/linuxdeepin/deepin-install-reborn/commit/1f767b68298232bf6ec70cb1c23ee46c80ca2387))
* **partition:**  partition name is not complete ([596f5f39](https://github.com/linuxdeepin/deepin-install-reborn/commit/596f5f3953fb055052c8493220d2a6fd14434e26))



<a name="2.4.4"></a>
### 2.4.4 (2018-11-02)


#### Bug Fixes

*   not set partition volume label ([e20f4499](https://github.com/linuxdeepin/deepin-install-reborn/commit/e20f4499135ffa9312bab6d21413a0870221ade1))



<a name="2.4.3"></a>
### 2.4.3 (2018-11-01)


#### Features

*   update resources ([e08df674](https://github.com/linuxdeepin/deepin-install-reborn/commit/e08df674329749d37136b43cac771338c4c1555a))
*   add user license ([7a45588a](https://github.com/linuxdeepin/deepin-install-reborn/commit/7a45588aed4d8738feec972e577cdafd337f2597))
*   update some config ([5c779f56](https://github.com/linuxdeepin/deepin-install-reborn/commit/5c779f56a717d86b4822a2aaec630f4a36388ae5))



<a name="2.4.2"></a>
### 2.4.2 (2018-10-25)


#### Features

*   generate grub theme ([cc765f56](https://github.com/linuxdeepin/deepin-install-reborn/commit/cc765f56b74b40d23d0862ac6c636ba6bedbb210))

#### Bug Fixes

*   empty partition page ([a0b1aea8](https://github.com/linuxdeepin/deepin-install-reborn/commit/a0b1aea8d34af243c39d66e26330715da718c626))



<a name="2.4.1"></a>
### 2.4.1 (2018-10-17)


#### Bug Fixes

*   missing QEvent header file ([68e60b99](https://github.com/linuxdeepin/deepin-install-reborn/commit/68e60b99dca7492b3f1bc7fba6ac52b2b582f9e9))



<a name=""></a>
##  2.4.0 (2018-10-16)


#### Features

*   update partition for sw ([d79a3f7a](https://github.com/linuxdeepin/deepin-install-reborn/commit/d79a3f7a2d864150f2e7b18a5d60b8d53daa1805))
*   remove skip simple partition ([581ef91f](https://github.com/linuxdeepin/deepin-install-reborn/commit/581ef91f1419b9f932556aa7312a839c04af8590))
*   update tr ([73b6d61f](https://github.com/linuxdeepin/deepin-install-reborn/commit/73b6d61f5bad3213b241418e79b803bf8591d41e))
*   add nomodeset kernel parameters when livecd have nomodeset ([6d11c345](https://github.com/linuxdeepin/deepin-install-reborn/commit/6d11c345018ee6eca9457b9fe90152eaebd78e51))
*   update full encrypt style ([f748f3ce](https://github.com/linuxdeepin/deepin-install-reborn/commit/f748f3ce0bc9db547ebe4742d579d5225257b622))
*   update checkbox style ([28de116c](https://github.com/linuxdeepin/deepin-install-reborn/commit/28de116cd0d42209d9c52f4dbed2d43d8028761a))
*   set full disk device and set crypt password ([059c96bd](https://github.com/linuxdeepin/deepin-install-reborn/commit/059c96bde6cbc2eb61d3d207558caa9f348388ab))
*   add encrypt frame ([4e3c388e](https://github.com/linuxdeepin/deepin-install-reborn/commit/4e3c388e3ca3a8675711133a28fde11b012b22f8))
*   support full disk encrypt ([37e09d80](https://github.com/linuxdeepin/deepin-install-reborn/commit/37e09d806488937a40945ef7aebaa9919a8e5a2c))
*   add GRUB_GFXPAYLOAD_LINUX=text ([bedfc930](https://github.com/linuxdeepin/deepin-install-reborn/commit/bedfc9305f2a50c2cb7ceb4dfee5165afd2ccd33))
*   support HiDPI ([f2e73872](https://github.com/linuxdeepin/deepin-install-reborn/commit/f2e7387297bd744198db22f7ebccb7965a77e8c6))

#### Bug Fixes

*   check empty password ([d02e86b5](https://github.com/linuxdeepin/deepin-install-reborn/commit/d02e86b55bef2b752baee1996f14a14e8304e3ea))
*   set boot flag, check root partition size ([f73ff4dd](https://github.com/linuxdeepin/deepin-install-reborn/commit/f73ff4dde64c9df945c914ce3a61a453e22569ff))
*   boot partition too small ([4659715b](https://github.com/linuxdeepin/deepin-install-reborn/commit/4659715bee27cc9a38fd4e8e4a5373d48c438e27))
*   mission partition frame pages ([83d980eb](https://github.com/linuxdeepin/deepin-install-reborn/commit/83d980ebf375365e5d1fdb422209b784047ac21d))
*   partition mode button style error ([74b11719](https://github.com/linuxdeepin/deepin-install-reborn/commit/74b11719429c009e59a55f16d3d214783618d852))
*   remove not require function ([a228f432](https://github.com/linuxdeepin/deepin-install-reborn/commit/a228f432f12271846b11028b38cc4cdfe0b472d6))
*   remove unused package for libwireshark ([c3eb9c91](https://github.com/linuxdeepin/deepin-install-reborn/commit/c3eb9c91dc2f87a89fc74b0e6d774840d6d0d383))
*   not run auto part script ([3c5cf439](https://github.com/linuxdeepin/deepin-install-reborn/commit/3c5cf439ba2fbef2c8cdbdf3b6218413429ec70c))



<a name=""></a>
##  2.3.8.5 (2018-10-22)




<a name=""></a>
##  2.3.8.4 (2018-10-18)


#### Bug Fixes

*   empty partition page ([b0a09c35](https://github.com/linuxdeepin/deepin-install-reborn/commit/b0a09c35f869d25ccc73baa51955d1a10027f2c0))



<a name=""></a>
##  2.3.8.3 (2018-10-18)




<a name=""></a>
##  2.3.8.2 (2018-10-11)


#### Features

*   don't skip simple partition and update partition list ([f5d7c343](https://github.com/linuxdeepin/deepin-install-reborn/commit/f5d7c34334a292ea91faf816f55fba811d0b08d1))



<a name=""></a>
##  2.3.8.1 (2018-09-27)


#### Bug Fixes

*   mission partition frame pages ([936174fe](https://github.com/linuxdeepin/deepin-install-reborn/commit/936174fe43fabcd78b84e152d429cc320f1ed8f0))
*   partition mode button style error ([26e8d01b](https://github.com/linuxdeepin/deepin-install-reborn/commit/26e8d01b7b8f7f472ef534c3697b33de902ae1bc))
*   boot partition too small ([0d9e9e6f](https://github.com/linuxdeepin/deepin-install-reborn/commit/0d9e9e6f63889e24bfaef9b57bc4342e8b5d2e80))



<a name="2.3.8"></a>
### 2.3.8 (2018-09-14)


#### Features

*   update mips inchroot script ([3146c66e](https://github.com/linuxdeepin/deepin-install-reborn/commit/3146c66eff592cfc16cc6b5f9718faa387ea5cc8))

#### Bug Fixes

*   not regenerate new machine id ([090626ab](https://github.com/linuxdeepin/deepin-install-reborn/commit/090626ab4fd66092b660801cb4e90fa6fdee997b))
*   lost partition label ([949dce15](https://github.com/linuxdeepin/deepin-install-reborn/commit/949dce15bb76c5755b99b66db7a3e186a508fdd0))



<a name="2.3.7"></a>
### 2.3.7 (2018-09-03)


#### Bug Fixes

*   not refresh gsettings schema ([78b3ef72](https://github.com/linuxdeepin/deepin-install-reborn/commit/78b3ef723ede586bde3668f778e42911ac944a06))
*   oem folder error ([94af35a3](https://github.com/linuxdeepin/deepin-install-reborn/commit/94af35a32d4a43e9dbe2259ab47c9882f5aca289))
* **hook:**  mount point invalid ([9e024af8](https://github.com/linuxdeepin/deepin-install-reborn/commit/9e024af869365cd5ce6bf7c720934e447a278e6e))

#### Features

*   update all version default settings ([9b9472a4](https://github.com/linuxdeepin/deepin-install-reborn/commit/9b9472a48b8696277725fca9d1cf25c46a0faddc))



<a name="2.3.6"></a>
### 2.3.6 (2018-08-16)




<a name="2.3.5"></a>
### 2.3.5 (2018-08-16)


#### Bug Fixes

*   error skip page ([b666f5f1](https://github.com/linuxdeepin/deepin-install-reborn/commit/b666f5f1bf6e3724309cb7ad135ee7a68335cf48))



<a name="2.3.4"></a>
### 2.3.4 (2018-08-16)


#### Bug Fixes

*   update professional default setting ([b61ffc42](https://github.com/linuxdeepin/deepin-install-reborn/commit/b61ffc4291c58459461eee5c9fca982452c12bf4))
*   check label exist ([4ffe5e3d](https://github.com/linuxdeepin/deepin-install-reborn/commit/4ffe5e3dce345007cb2e7ec652c81ab4c1f5b986))



<a name="2.3.3"></a>
### 2.3.3 (2018-08-15)


#### Features

*   just shutdown in oem mode ([f674d881](https://github.com/linuxdeepin/deepin-install-reborn/commit/f674d881be9de3df35a1ee763418d6fab149fae3))
*   only professional version have user license ([0d43022f](https://github.com/linuxdeepin/deepin-install-reborn/commit/0d43022f3a6a070f7effeabc439642aebaec65b6))

#### Bug Fixes

*   indicator size error ([34f7594d](https://github.com/linuxdeepin/deepin-install-reborn/commit/34f7594d6c517d56e099be1495c1002dd8d59b67))



<a name="2.3.2"></a>
### 2.3.2 (2018-08-14)


#### Bug Fixes

*   set label error ([db33a14b](https://github.com/linuxdeepin/deepin-install-reborn/commit/db33a14b1465658dbe91d1b42af8eb14094140aa))



<a name="2.3.1"></a>
### 2.3.1 (2018-08-13)


#### Features

*   add user license ([089cac4f](https://github.com/linuxdeepin/deepin-install-reborn/commit/089cac4fdcc7ecbb065b13d3efc03e9534e78378))
*   skip timezone when open after_reboot_config ([4a0d32f5](https://github.com/linuxdeepin/deepin-install-reborn/commit/4a0d32f558f82827db4198834c005df078bec200))
*   add end-user license agreement ([a42ded6a](https://github.com/linuxdeepin/deepin-install-reborn/commit/a42ded6a2963f0e309c5780f191f9281db73134b))

#### Bug Fixes

*   error skip disk page ([4938987e](https://github.com/linuxdeepin/deepin-install-reborn/commit/4938987eccaf638a7b1a19b0b3922e04c92ace28))
*   not skip system info when open after_reboot_config ([468d73e9](https://github.com/linuxdeepin/deepin-install-reborn/commit/468d73e96226a3af1dbc8e520f2c8ea3e5b545c7))
*   start prematurely ([25423875](https://github.com/linuxdeepin/deepin-install-reborn/commit/2542387550d142e2506da11536debc685c108456))



<a name=""></a>
##  2.3.0 (2018-08-08)


#### Bug Fixes

* **FullInstall:**  root partition offset error ([69bf3462](https://github.com/linuxdeepin/deepin-install-reborn/commit/69bf3462172fcd8efff481213352717410aab6cf))



<a name="2.2.19"></a>
### 2.2.19 (2018-08-07)


#### Bug Fixes

*   precision problem when calculating swap ([c4fded0f](https://github.com/linuxdeepin/deepin-install-reborn/commit/c4fded0ff4d2a2ba0177590d67ce51928fd8a07b))
*   wrong tip width ([1bc3da24](https://github.com/linuxdeepin/deepin-install-reborn/commit/1bc3da2484840137ee37993e37f3eb0cd9eb9071))
*   unable to display partition number correctly ([ed501c90](https://github.com/linuxdeepin/deepin-install-reborn/commit/ed501c90ebc089bf86d7e20ac0e32649d67bc8ab))

#### Features

*   support disk label for full disk install mode ([5def6bad](https://github.com/linuxdeepin/deepin-install-reborn/commit/5def6bad9a021eeb795dbeb803c57e02bb885a32))
*   support auto swap ([b48f4b8b](https://github.com/linuxdeepin/deepin-install-reborn/commit/b48f4b8bcdae27f17cc72eb9fbbd6cf1740a3e53))
*   modify the order of installation ([f4b549da](https://github.com/linuxdeepin/deepin-install-reborn/commit/f4b549da49eec1a78607f95874933b82f903b46e))
* **disk:**  change simple mode to first ([a0e9689f](https://github.com/linuxdeepin/deepin-install-reborn/commit/a0e9689fc7d8a545adde4c975c267fa0b8ef9530))


2018-07-31 2.2.18 haruyukilxz <lxz@ilxz.me>
#### Bug Fixes

*   no grub item of live-filesystem on sw ([707fa0b9](707fa0b9))
*   missing live-fileysystem uuid on sw ([8c014f8e](8c014f8e))
*   Adapt lintian ([7f04a6c2](7f04a6c2))
* **tip:**  wrong hints are not newline ([81384c5d](81384c5d))

#### Features

*   set default password for livefs. ([235be0cc](235be0cc))
*   add arm64-efi support ([097258d4](097258d4))
*   add cryptroot support ([ec8d136e](ec8d136e))

2018-06-07 2.2.17 Xu Shaohua <xushaohua2016@outlook.com>
    * Add screen brightness shortcut
    * Support empty password
    * Hide timezone page if it is disabled

2018-05-28 2.2.16.1 deepinzhangshuang <zhangshuang@deepin.com>
    * fix /etc/default/grub GRUB_DISTRIBUTOR
    * Add live-filesystem grub menu on sw
    * fix symlinks in /boot/boot on sw

2018-05-14 2.2.16 Xu Shaohua <xushaohua2016@outlook.com>
    * Fix live-filesystem config for sw and loongson
    * Set sw421 sources.list to as4s

2018-03-11 2.2.15.3 Xu Shaohua <xushaohua@deepin.com>
    * Check existence of /etc/deepin-version

2018-04-10 2.2.15.2 Xu Shaohua <xushaohua@deepin.com>
    * Fix timestamp issue of APT cache packages
    * Add installer user form
    * Add resolution settings for grub on arm64

2018-03-16 2.2.15.1 Xu Shaohua <xushaohua@deepin.com>
    * Set password for live environment

2018-03-15 2.2.15 Xu Shaohua <xushaohua@deepin.com>
    * Add arm64 source file list
    * Add settings for professional edition
    * Fix shell script syntax error
    * Update i18n

2018-01-25 2.2.14 Xu Shaohua <xushaohua@deepin.com>
	* Support arm64

2017-11-29 2.2.13 Xu Shaohua <xushaohua@deepin.com>
	* Update grub based on current locale

2017-11-29 2.2.12 Xu Shaohua <xushaohua@deepin.com>
	* Uninstall live-filesystem in lupin mode

2017-11-28 2.2.11 Xu Shaohua <xushaohua@deepin.com>
	* Remove live-filesystem package for lupin

2017-11-22 2.2.10 Xu Shaohua <xushaohua@deepin.com>
	* Generate font cache for wine QQ

2017-11-22 2.2.9 Xu Shaohua <xushaohua@deepin.com>
	* Remove target character file before calling mknod()

2017-11-20 2.2.8 Xu Shaohua <xushaohua@deepin.com>
	* 更新翻译
	* 修改自动分区时, boot 分区最小容量为 500M
	* 分区时, 只要在必要时再遍历home目录修改属性

2017-11-07 2.2.7 Xu Shaohua <xushaohua@deepin.com>
	* installer-setting 支持保存 StringList
	* 过滤 dde_launcher_hold_packages

2017-11-07 2.2.6 Xu Shaohua <xushaohua@deepin.com>
	* 更新翻译
	* 支持批量激活
	* 修正软件包依赖关系
	* 修正用户名错误提示
	* 提示用户弹出安装设备
