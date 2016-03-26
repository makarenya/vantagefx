    var emptySign = '',
        operaStr = 'opera',
        gecko18Str = 'gecko1_8',
        ie9Str = 'ie9',
        safariStr = 'safari',
        ie6Str = 'ie6',
        ie8Str = 'ie8',
        Qb = '2A2A70BABA4AB9508D7BA747D2B08EA3',
        Rb = '3D90D7A4F32798207AB67A6028D39C03',
        Sb = '6ABA94B32D3FD34568DE2E4AAA463373',
        Tb = '97A60A56BD971D60069316AF35BCDFB9',
        cb97Str = 'CB970908EEF37EF1DF1F4C92FAA58364',
        ed9cStr = 'ED9C30D085445CFD2A8F592CA143A909';


var x = [];

function G(browserNameArray, someHash) {
    var c = x;
    for (var d = 0, e = browserNameArray.length - 1; d < e; ++d) {
        c = c[browserNameArray[d]] || (c[browserNameArray[d]] = [])
    }
    c[browserNameArray[e]] = someHash
}


G([operaStr], Qb);
G([gecko18Str], Rb);
G([ie9Str], Sb);
G([safariStr], Tb);
G([ie8Str], cb97Str);
G([ie6Str], ed9cStr);


console.log(x);