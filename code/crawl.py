import requests  #爬虫库
import re        #正则库
import os        #系统库

counter = 0      #每个网页爬取的图片数量
per_need = 20     #每个网页需要爬的图片数量
num_now = 1      #当前已爬图片总数
num_need = 10000 #需要爬取的图片总数
html_list = []   #栈结构，来存网页链接
bf = []          #布隆过滤器数组，即BF
k = 6            #Hash函数个数
m = 16000        #BF数组的长度
n = 1000         #集合元素总数，即1000个网站

###根据网页的url，得到网页的HTML文本
def get_html_text(url):
    try:
        r = requests.get(url, timeout=30)     #使用get请求将网页提取为一个response对象
        r.raise_for_status()                  #查看是否访问成功
        r.encoding = r.apparent_encoding      #改变头的编码，使得整个源代码可读
        return r.text                         #返回网页的源代码文本
    except:
        return ""

###正则表达式匹配HTML文本中的图片
def find_picture(text):
    return re.findall(r'http://.*?\.jpg',text)  #在文本中匹配所有图片url，返回一个列表

###将图片保存到本地文件夹
def save_picture(url):
    global num_now
    global num_need
    root = "C://Users//HP//Desktop//picture//"   #指定图片保存的目录路径
    path = root + str(num_now) + ".jpg"          #对图片重命名，方便查看
    try:
        if num_now <= num_need:                  #检查是否已爬到足够的图片
            num_now = num_now + 1                
            if not os.path.exists(root):         #检查文件夹是否存在
                os.mkdir(root)
            if not os.path.exists(path):         #检查图片是否已存在
                r = requests.get(url)
                with open(path, 'wb') as file:   #打开文件夹并写入图片
                    file.write(r.content)
                    file.close()
                    print("文件保存成功")
            else:
                print("文件已存在")
        else:
            return
    except:
        print("爬取失败")

###正则表达式匹配HTML文本中的子网页
def find_childweb(text):
    return re.findall(r'/cng/.*?\.htm',text)   #在文本中匹配所有子网页url，返回一个列表

###根据网页的url,爬下来一个网页的需要的图片
def crawl_picture(url):
    global counter
    global per_need
    picture_list = []                          #新建一个列表来保存图片的url
    html_text = get_html_text(url)
    picture_list = find_picture(html_text)
    for i in picture_list:                     #循环的方式来保存图片，控制保存的数量
        counter = counter + 1
        if counter > per_need:
            counter = 0
            return
        save_picture(i)

###根据网页的url,爬取所有的子网页的url
def crawl_childweb(url):
    child_list = []                           #新建一个列表来保存子网页的url
    html_text = get_html_text(url)              
    child_list = find_childweb(html_text)      
    return child_list                         #返回这个爬取到的所有子网页的url   

###爬取整个网站所需的图片，通过栈方法，本质上是深度优先算法
def crawl_all():                            
    global html_list
    global num_now
    global num_need
    if num_now <= num_need:
        if html_list != []:                   #判断栈是否空，即是否爬玩所有网页
            now = html_list.pop()             #取出栈顶元素。
            print(now)
            crawl_picture(now)                #爬取当前网页的图片
            child_list = crawl_childweb(now)  #爬取当前网页的子网页，保存为列表
            for i in child_list:              #通过BF的Hash来过滤已经爬过的网页，将没有爬过的进栈
                s = 'http://www.dili360.com' + i
                if Hash(s) == 0:              
                    print(Hash(s))
                    html_list.append(s)
            crawl_all()                       #递归调用继续爬取
        else:
            print("爬取结束")
    else:
        return

###6个Hash函数定义
def RSHash(str):
    global m
    b = 378551
    a = 63689
    hashing = 0
    for i in range(len(str)):
        hashing = hashing * a + ord(str[i])
        a = a * b
    return hashing%m

def JSHash(str):
    global m
    hashing = 1315423911
    for i in range(len(str)):
        hashing ^= (hashing << 5) + ord(str[i]) + (hashing >> 2)
    return hashing%m

def ELFHash(str):
    global m
    hashing = 0
    x = 0
    for i in range(len(str)):
        hashing = (hashing << 4) + ord(str[i])
        x = hashing & 0xf000000
        if x != 0:
            hashing ^= (x >> 24)
        hashing &= ~x
    return hashing%m

def BKDRHash(str):
    global m
    seed = 131
    hashing = 0
    for i in range(len(str)):
        hashing = (hashing * seed) + ord(str[i])
    return hashing%m

def SDBMHash(str):
    global m
    hashing = 0
    for i in range(len(str)):
        hashing = ord(str[i]) + (hashing << 6) + (hashing << 16) - hashing
    return hashing%m

def DJBHash(str):
    global m
    hashing = 5381
    for i in range(len(str)):
        hashing = ((hashing << 5) + hashing) + ord(str[i])
    return hashing%m

###对一个url进行Hash，修改BF数组并返回结果
def Hash(str):
    global bf
    result = 0
    if bf[RSHash(str)] == 1:      #如果映射全是1，则说明已经出现过
        if bf[JSHash(str)] == 1:
            if bf[ELFHash(str)] == 1:
                if bf[BKDRHash(str)] == 1:
                    if bf[SDBMHash(str)] == 1:
                        if bf[DJBHash(str)] == 1:
                            result = 1
    if result == 0:               #修改BF数组
        bf[RSHash(str)] = 1
        bf[JSHash(str)] = 1
        bf[ELFHash(str)] = 1
        bf[BKDRHash(str)] = 1
        bf[SDBMHash(str)] = 1
        bf[DJBHash(str)] = 1
    return result                 #返回判断结果

###初始化BF数组        
def init_bf():
    global bf
    global m
    for i in range(m):            #通过循环置0                    
        bf.append(0)

###main函数设置爬取的主网站链接        
def main():
    global html_list
    init_bf()                     #初始化BF
    url = 'http://www.dili360.com/cng/pic/index.htm' #指定主网页
    html_list.append(url)         #将主网页进栈
    crawl_all()                   #开始爬取
    
main()
