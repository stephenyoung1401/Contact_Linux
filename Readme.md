<!--
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-25 20:29:51
 * @LastEditTime: 2019-03-25 21:52:13
 -->

# Contact

## Start:
``` shell

sudo chmod 777 start.sh
make dirctly
G```


## Functions:
* a beta version
  * the whole make file undo
* support c/s
  * use JSON to delivery command
  * modify config.ini to
    * add ACL cmd
    * choose server ip and port
* regex search 
  * it will match pattern like __.\*[info].\*__
  * it search each pharse include (name phone description) and __sorted__ by the order above
* support local mode
  * use local database (sqlite3 -- embedded)
  * also can be modified by config.ini
* UI is not friendly but detached from controller ,so you can 
  * make an amazing UI with interface supported
  * use other input method like touching
* data departed from data structure and 
  * will be more easier to optimize
  * add new property to person info