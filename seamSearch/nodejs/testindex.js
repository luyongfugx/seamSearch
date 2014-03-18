var seamSearch= require('./build/Release/seamSearch');
var indexData=[[{'name':'t','type':'long','value':'1'},{'name':'tx','type':'str','value':'hello 还岑'}],
    [{'name':'t','type':'long','value':'14'},{'name':'tx','type':'str','value':'hello wayne'}],
    [{'name':'t','type':'long','value':'13'},{'name':'tx','type':'str','value':'hello freda liu'}]]

var status= seamSearch.index('/Users/waynelu/nginxhtmls/luyfsearch/index/','/Users/waynelu/nginxhtmls/seamSearch/seamSearch/words.dict',
    indexData);
console.log(status);
