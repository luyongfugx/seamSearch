seamSearch
==========



     这几年nodejs越来越火爆，js程序员们终于可以不局限于浏览器那一亩三分地了，很多以前由java,c等实现的东西纷纷有了nodejs版本，我也随波逐流一把，写了一个nodejs的搜索引擎addon,现在已经放github:https://github.com/luyongfugx/seamSearch,
这个开源addon其实是对我几年前写的c++ 版的luyfsearch(github:https://github.com/luyongfugx/luyfsearch)进行的一个nodejs封装。下面我们来看看怎么使用:
     1.首先当然是安装，这个很简单：
        npm install seamsearch
      2.建立索引
        新建testindex.js文件，内容如下：
        var seamSearch= require('seamsearch');
        //需要索引的数据,用一个数组表示。每个数组项又是一个数组，表示一个document(lucene里面的document同理)，这里的每            个数组项表示一个字段，分别由name,type,value三个属性组成，分别表示字段名称，字段类型（long，str两种），字段内容。以下说明建立2个document的索引。
        var indexData=[[{'name':'t','type':'long','value':'1'},{'name':'tx','type':'str','value':'hello seam'}],
        [{'name':'t','type':'long','value':'14'},{'name':'tx','type':'str','value':'hello wayne'}],
        [{'name':'t','type':'long','value':'13'},{'name':'tx','type':'str','value':'hello freda'}]];
 用seamSearch.index方法来建立索引，第一个参数是索引文件路径，第二个参数是字典路径,字典文字由一个文本文件组成，每行是一个词，第三个参数是要索引数据
        var status= seamSearch.index('/Users/waynelu/nginxhtmls/luyfsearch/index/','/Users/waynelu/nginxhtmls/seamSearch/seamSearch/words.dict',indexData);
        console.log(status);
     运行node testindex.js 运行结果如下：
    create index!
    { status: 'succ' }
说明索引建立成功，这时候到索引文件目录下看，能看到.fdt .del等文件，这些都是索引文件
     
      3.搜索
      新建testsearch.js文件, 内容如下：
        var seamSearch = require('seamsearch');
        var ts=[['t','long'],['tx','str']];//索引document结构,和建立索引时候的索引结构对应。
        用seamSearch.search方法进行搜索，第一个参数是索引文件路径，第二个参数是字典路径,字典文字由一个文本文件组成，每行是一个词.第三个参数是要搜索的词，我们这里搜索'hello',第四个参数是要读取的字段和类型。
        var searchresult=seamSearch.search('/Users/waynelu/nginxhtmls/luyfsearch/index/',
            '/Users/waynelu/nginxhtmls/seamSearch/seamSearch/words.dict',
            'hello',ts);
        console.log(searchresult);
运行node testsearch.js,结果如下
{ hits: 3,
  datas: 
   [ { id: 0, t: 1, tx: 'hello seam' },
     { id: 1, t: 14, tx: 'hello wayne' },
     { id: 2, t: 13, tx: 'hello freda' } ] }
搜索成功，返回一个json数据，hits说明搜索命中3个document,datas是搜索到的3个document.

到这里一切ok.

再说一下，这个项目的地址是：
https://github.com/luyongfugx/seamSearch
觉得不错的话不妨去给个star :)

       
