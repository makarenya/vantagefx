function Basic() {
    var emptySign = '',
        forGwtOnLoadErrorFnStr = '" for "gwt:onLoadErrorFn"',
        forGwtOnPropertyErrorFnStr = '" for "gwt:onPropertyErrorFn"',
        scriptEndStr = '"><\/script>',
        numberSign = '#',
        cacheHtmlStr = '.cache.html',
        slashSign = '/',
        commentSign = '//',
        Qb = '2A2A70BABA4AB9508D7BA747D2B08EA3',
        Rb = '3D90D7A4F32798207AB67A6028D39C03',
        Sb = '6ABA94B32D3FD34568DE2E4AAA463373',
        Tb = '97A60A56BD971D60069316AF35BCDFB9',
        doubleColonSign = ':',
        PrefixSign = '::',
        scriptOnInjectionDoneStr = '<script defer="defer">Basic.onInjectionDone(\'Basic\')<\/script>',
        scriptIdStr = '<script id="',
        equalSign = '=',
        questionSign = '?',
        badHandlerStr = 'Bad handler "',
        basicStr = 'Basic',
        basicNocacheJsStr = 'Basic.nocache.js',
        basicPrefixStr = 'Basic::',
        cb97Str = 'CB970908EEF37EF1DF1F4C92FAA58364',
        DOMContentLoadedStr = 'DOMContentLoaded',
        ed9cStr = 'ED9C30D085445CFD2A8F592CA143A909',
        scriptUCStr = 'SCRIPT',
        gwtMarkerBasicStr = '__gwt_marker_Basic',
        baseStr = 'base',
        baseUrlStr = 'baseUrl',
        beginStr = 'begin',
        bootstrapStr = 'bootstrap',
        clearCacheGifStr = 'clear.cache.gif',
        contentStr = 'content',
        endStr = 'end',
        gechoStr = 'gecko',
        gecko18Str = 'gecko1_8',
        gwtCodesvrStr = 'gwt.codesvr=',
        gwtHostedStr = 'gwt.hosted=',
        gwtHybridStr = 'gwt.hybrid',
        gwtOnLoadErrorFn = 'gwt:onLoadErrorFn',
        gwtOnPropertyErrorFnStr = 'gwt:onPropertyErrorFn',
        gwtPropertyStr = 'gwt:property',
        hostedHtmlQBasicStr = 'hosted.html?Basic',
        ie6Str = 'ie6',
        ie8Str = 'ie8',
        ie9Str = 'ie9',
        iFrameStr = 'iframe',
        imgStr = 'img',
        javascriptNoneStr = "javascript:''",
        loadExternalRefsStr = 'loadExternalRefs',
        metaStr = 'meta',
        moduleRequestedStr = 'moduleRequested',
        moduleStartupStr = 'moduleStartup',
        msieStr = 'msie',
        nameStr = 'name',
        operaStr = 'opera',
        iFrameStyleStr = 'position:absolute;width:0;height:0;border:none',
        safariStr = 'safari',
        scriptStr = 'script',
        selectingPermutationStr = 'selectingPermutation',
        startupStr = 'startup',
        undefinedStr = 'undefined',
        unknownStr = 'unknown',
        userAgentStr = 'user.agent',
        webkitStr = 'webkit';
    
    
    var theWindow = window,
        theDocument = document,
        o = theWindow.__gwtStatsEvent ? function (a) { return theWindow.__gwtStatsEvent(a) } : null,
        p = theWindow.__gwtStatsSessionId ? theWindow.__gwtStatsSessionId : null,
        q,
        r,
        documentAlredyLoaded,
        t = emptySign,
        u = {},
        knownUserAgents = [],
        detectedUserAgent = [],
        browserHashAssignment = [],
        y = 0,
        z,
        A;

    o && o({moduleName: basicStr, sessionId: p, subSystem: startupStr, evtGroup: bootstrapStr, millis: (new Date).getTime(), type: beginStr});
    if (!theWindow.__gwt_stylesLoaded) {
        theWindow.__gwt_stylesLoaded = {}
    }
    if (!theWindow.__gwt_scriptsLoaded) {
        theWindow.__gwt_scriptsLoaded = {}
    }

    function isLoadedInsideGwtEnvironment() {
        var b = false;
        try {
            var c = theWindow.location.search;
            return (c.indexOf(gwtCodesvrStr) != -1 || (c.indexOf(gwtHostedStr) != -1 || theWindow.external && theWindow.external.gwtOnLoad)) && c.indexOf(gwtHybridStr) == -1
        } catch (a) {
        }
        isLoadedInsideGwtEnvironment = function () {
            return b
        };
        return b
    }

    function C() {
        if (q && r) {
            var theIFrame = theDocument.getElementById(basicStr);
            var iframeWindow = theIFrame.contentWindow;
            if (isLoadedInsideGwtEnvironment()) {
                iframeWindow.__gwt_getProperty = function (a) {
                    return getCurrentProperty(a)
                }
            }
            Basic = null;
            iframeWindow.gwtOnLoad(z, basicStr, t, y);
            o && o({moduleName: basicStr, sessionId: p, subSystem: startupStr, evtGroup: moduleStartupStr, millis: (new Date).getTime(), type: endStr})
        }
    }

    function D() {
        function e(a) {
            var b = a.lastIndexOf(numberSign);
            if (b == -1) {
                b = a.length
            }
            var c = a.indexOf(questionSign);
            if (c == -1) {
                c = a.length
            }
            var d = a.lastIndexOf(slashSign, Math.min(c, b));
            return d >= 0 ? a.substring(0, d + 1) : emptySign
        }

        function f(a) {
            if (a.match(/^\w+:\/\//)) {
            } else {
                var b = theDocument.createElement(imgStr);
                b.src = a + clearCacheGifStr;
                a = e(b.src)
            }
            return a
        }

        function g() {
            var a = F(baseUrlStr);
            if (a != null) {
                return a
            }
            return emptySign
        }

        function h() {
            var a = theDocument.getElementsByTagName(scriptStr);
            for (var b = 0; b < a.length; ++b) {
                if (a[b].src.indexOf(basicNocacheJsStr) != -1) {
                    return e(a[b].src)
                }
            }
            return emptySign
        }

        function i() {
            var a;
            if (typeof isBodyLoaded == undefinedStr || !isBodyLoaded()) {
                var b = gwtMarkerBasicStr;
                var c;
                theDocument.write(scriptIdStr + b + scriptEndStr);
                c = theDocument.getElementById(b);
                a = c && c.previousSibling;
                while (a && a.tagName != scriptUCStr) {
                    a = a.previousSibling
                }
                if (c) {
                    c.parentNode.removeChild(c)
                }
                if (a && a.src) {
                    return e(a.src)
                }
            }
            return emptySign
        }

        function j() {
            var a = theDocument.getElementsByTagName(baseStr);
            if (a.length > 0) {
                return a[a.length - 1].href
            }
            return emptySign
        }

        function k() {
            var a = theDocument.location;
            return a.href == a.protocol + commentSign + a.host + a.pathname + a.search + a.hash
        }

        var l = g();
        if (l == emptySign) {
            l = h()
        }
        if (l == emptySign) {
            l = i()
        }
        if (l == emptySign) {
            l = j()
        }
        if (l == emptySign && k()) {
            l = e(theDocument.location.href)
        }
        l = f(l);
        t = l;
        return l
    }

    function E() {
        var metas = document.getElementsByTagName(metaStr); // meta
        for (var c = 0, d = metas.length; c < d; ++c) {
            var theMeta = metas[c],
                metaName = theMeta.getAttribute(nameStr),
                g;

            if (metaName) {
                metaName = metaName.replace(basicPrefixStr, emptySign);
                if (metaName.indexOf(PrefixSign) >= 0) {
                    continue
                }
                if (metaName == gwtPropertyStr) {
                    g = theMeta.getAttribute(contentStr);
                    if (g) {
                        var h, i = g.indexOf(equalSign);
                        if (i >= 0) {
                            metaName = g.substring(0, i);
                            h = g.substring(i + 1)
                        } else {
                            metaName = g;
                            h = emptySign
                        }
                        u[metaName] = h
                    }
                } else if (metaName == gwtOnPropertyErrorFnStr) {
                    g = theMeta.getAttribute(contentStr);
                    if (g) {
                        try {
                            A = eval(g)
                        } catch (a) {
                            alert(badHandlerStr + g + forGwtOnPropertyErrorFnStr)
                        }
                    }
                } else if (metaName == gwtOnLoadErrorFn) {
                    g = theMeta.getAttribute(contentStr);
                    if (g) {
                        try {
                            z = eval(g)
                        } catch (a) {
                            alert(badHandlerStr + g + forGwtOnLoadErrorFnStr)
                        }
                    }
                }
            }
        }
    }

    function F(a) {
        var b = u[a];
        return b == null ? null : b
    }

    function assignHashToBrowserList(browserNameArray, someHash) {
        var browserHash = browserHashAssignment;
        for (var d = 0, e = browserNameArray.length - 1; d < e; ++d) {
            browserHash = browserHash[browserNameArray[d]] || (browserHash[browserNameArray[d]] = [])
        }
        browserHash[browserNameArray[e]] = someHash
    }

    function getCurrentProperty(selector) {
        var detectedAgent = detectedUserAgent[selector](),
            knownAgents = knownUserAgents[selector];
        if (detectedAgent in knownAgents) {
            return detectedAgent
        }
        var valueKeyHash = [];
        for (var key in knownAgents) {
            valueKeyHash[knownAgents[key]] = key
        }
        if (A) {
            A(selector, valueKeyHash, detectedAgent)
        }
        throw null
    }

    var iframeAlreadyCreated;

    function createEmptyIFrame() {
        if (!iframeAlreadyCreated) {
            iframeAlreadyCreated = true;
            var theIFrame = theDocument.createElement(iFrameStr);
            theIFrame.src = javascriptNoneStr;
            theIFrame.id = basicStr;
            theIFrame.style.cssText = iFrameStyleStr;
            theIFrame.tabIndex = -1;
            theDocument.body.appendChild(theIFrame);
            o && o({moduleName: basicStr, sessionId: p, subSystem: startupStr, evtGroup: moduleStartupStr, millis: (new Date).getTime(), type: moduleRequestedStr});
            theIFrame.contentWindow.location.replace(t + L)
        }
    }

    detectedUserAgent[userAgentStr] = function () {
        var theUserAgent = navigator.userAgent.toLowerCase();
        var createVersion = function (a) {
            return parseInt(a[1]) * 1000 + parseInt(a[2])
        };
        if (function () {
                return theUserAgent.indexOf(operaStr) != -1
            }())return operaStr;
        if (function () {
                return theUserAgent.indexOf(webkitStr) != -1
            }())return safariStr;
        if (function () {
                return theUserAgent.indexOf(msieStr) != -1 && theDocument.documentMode >= 9
            }())return ie9Str;
        if (function () {
                return theUserAgent.indexOf(msieStr) != -1 && theDocument.documentMode >= 8
            }())return ie8Str;
        if (function () {
                var a = /msie ([0-9]+)\.([0-9]+)/.exec(theUserAgent);
                if (a && a.length == 3)return createVersion(a) >= 6000
            }())return ie6Str;
        if (function () {
                return theUserAgent.indexOf(gechoStr) != -1
            }())return gecko18Str;
        return unknownStr
    };
    knownUserAgents[userAgentStr] = {gecko1_8: 0, ie6: 1, ie8: 2, ie9: 3, opera: 4, safari: 5};
    Basic.onScriptLoad = function () {
        if (iframeAlreadyCreated) {
            r = true;
            C()
        }
    };
    Basic.onInjectionDone = function () {
        q = true;
        o && o({moduleName: basicStr, sessionId: p, subSystem: startupStr, evtGroup: loadExternalRefsStr, millis: (new Date).getTime(), type: endStr});
        C()
    };
    E();
    D();
    var browserAgentAssignedHash;
    var L;
    if (isLoadedInsideGwtEnvironment()) {
        if (theWindow.external && (theWindow.external.initModule && theWindow.external.initModule(basicStr))) {
            theWindow.location.reload();
            return
        }
        L = hostedHtmlQBasicStr;
        browserAgentAssignedHash = emptySign
    }
    o && o({moduleName: basicStr, sessionId: p, subSystem: startupStr, evtGroup: bootstrapStr, millis: (new Date).getTime(), type: selectingPermutationStr});
    if (!isLoadedInsideGwtEnvironment()) {
        try {
            assignHashToBrowserList([operaStr], Qb);
            assignHashToBrowserList([gecko18Str], Rb);
            assignHashToBrowserList([ie9Str], Sb);
            assignHashToBrowserList([safariStr], Tb);
            assignHashToBrowserList([ie8Str], cb97Str);
            assignHashToBrowserList([ie6Str], ed9cStr);
            browserAgentAssignedHash = browserHashAssignment[getCurrentProperty(userAgentStr)];
            var M = browserAgentAssignedHash.indexOf(doubleColonSign);
            if (M != -1) {
                y = Number(browserAgentAssignedHash.substring(M + 1));
                browserAgentAssignedHash = browserAgentAssignedHash.substring(0, M)
            }
            L = browserAgentAssignedHash + cacheHtmlStr
        } catch (a) {
            return
        }
    }
    var N;

    function documentLoadingComplete() {
        if (!documentAlredyLoaded) {
            documentAlredyLoaded = true;
            C();
            if (theDocument.removeEventListener) {
                theDocument.removeEventListener(DOMContentLoadedStr, documentLoadingComplete, false)
            }
            if (N) {
                clearInterval(N)
            }
        }
    }

    if (theDocument.addEventListener) {
        theDocument.addEventListener(DOMContentLoadedStr, function () {
            createEmptyIFrame();
            documentLoadingComplete()
        }, false)
    }
    var N = setInterval(function () {
        if (/loaded|complete/.test(theDocument.readyState)) {
            createEmptyIFrame();
            documentLoadingComplete()
        }
    }, 50);
    o && o({moduleName: basicStr, sessionId: p, subSystem: startupStr, evtGroup: bootstrapStr, millis: (new Date).getTime(), type: endStr});
    o && o({moduleName: basicStr, sessionId: p, subSystem: startupStr, evtGroup: loadExternalRefsStr, millis: (new Date).getTime(), type: beginStr});
    theDocument.write(scriptOnInjectionDoneStr)
}
Basic();