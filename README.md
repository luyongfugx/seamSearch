# seamSearch
==========

这几年 nodejs 越来越火爆， js 程序员们终于可以不局限于浏览器那一亩三分地了，很多以前由 java, c 等实现的东西纷纷有了 nodejs 版本，我也随波逐流一把，写了一个 nodejs 的搜索引擎 addon,现在已经放 github:https://github.com/luyongfugx/seamSearch,
这个开源 addon 其实是对我几年前写的 c++ 版的 luyfsearch(github:https://github.com/luyongfugx/luyfsearch) 进行的一个 nodejs 封装。下面我们来看看怎么使用:

1. 首先当然是安装，这个很简单：

```
npm install seamsearch
```

2. 建立索引

新建 testindex.js 文件，内容如下：

```js
var seamSearch = require('seamsearch');
//需要索引的数据,用一个数组表示。每个数组项又是一个数组，表示一个 document( lucene 里面的 document 同理)，这里的每个数组项表示一个字段，分别由name, type, value三个属性组成，分别表示字段名称，字段类型（ long，str 两种），字段内容。以下说明建立 2 个 document 的索引。
var indexData = [
    [{'name':'t','type':'long','value':'1'},{'name':'tx','type':'str','value':'hello seam'}],
    [{'name':'t','type':'long','value':'14'},{'name':'tx','type':'str','value':'hello wayne'}],
    [{'name':'t','type':'long','value':'13'},{'name':'tx','type':'str','value':'hello freda'}]
];

// 用 seamSearch.index 方法来建立索引，第一个参数是索引文件路径，第二个参数是字典路径,字典文字由一个文本文件组成，每行是一个词，第三个参数是要索引数据
var status = seamSearch.index('/Users/waynelu/nginxhtmls/luyfsearch/index/','/Users/waynelu/nginxhtmls/seamSearch/seamSearch/words.dict',indexData);
console.log(status);
```

运行 `node testindex.js` 运行结果如下：

```
create index!
{ status: 'succ' }
```

说明索引建立成功，这时候到索引文件目录下看，能看到 `.fdt` `.del`等文件，这些都是索引文件
     
3. 搜索

新建 `testsearch.js` 文件, 内容如下：

```js
var seamSearch = require('seamsearch');
var ts = [['t','long'],['tx','str']];//索引document结构,和建立索引时候的索引结构对应。
用seamSearch.search方法进行搜索，第一个参数是索引文件路径，第二个参数是字典路径,字典文字由一个文本文件组成，每行是一个词.第三个参数是要搜索的词，我们这里搜索'hello',第四个参数是要读取的字段和类型。
var searchresult=seamSearch.search(
    '/Users/waynelu/nginxhtmls/luyfsearch/index/',
    '/Users/waynelu/nginxhtmls/seamSearch/seamSearch/words.dict',
    'hello',
    ts
);
console.log(searchresult);
```

运行 `node testsearch.js`, 结果如下：

```
{ hits: 3,
    datas: 
    [ { id: 0, t: 1, tx: 'hello seam' },
        { id: 1, t: 14, tx: 'hello wayne' },
        { id: 2, t: 13, tx: 'hello freda' } ] }
```

搜索成功，返回一个 json 数据，hits 说明搜索命中3个 document , datas 是搜索到的 3 个 document.

到这里一切ok.

再说一下，这个项目的地址是：
https://github.com/luyongfugx/seamSearch
觉得不错的话不妨去给个star :)
