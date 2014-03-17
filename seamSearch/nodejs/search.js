var seamSearch = require('./build/Release/seamSearch');
var ts=[['t','long'],['tx','str']];
var searchresult=seamSearch.search('/Users/waynelu/nginxhtmls/luyfsearch/index/',
    '/Users/waynelu/nginxhtmls/seamSearch/seamSearch/words.dict',
    '还岑',ts);
console.log(searchresult);

