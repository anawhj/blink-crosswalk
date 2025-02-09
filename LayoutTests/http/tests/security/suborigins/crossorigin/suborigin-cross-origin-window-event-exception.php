<?php
header("Content-Security-Policy: suborigin foobar");
?>
<!DOCTYPE html>
<html>
<head>
</head>
<body>
    <iframe src="http://127.0.0.1:8000/"></iframe>
    <script src="/js-test-resources/js-test.js"></script>
    <script>
        description("Cross-origin access to 'window.event' should throw a SecurityError.");
        window.jsTestIsAsync = true;

        var frame = document.querySelector('iframe');
        window.onload = function () {
            shouldThrow("frame.contentWindow.event", '"SecurityError: Blocked a frame with origin \\"suborigin+foobar+http://127.0.0.1:8000\\" from accessing a cross-origin frame."');
            shouldThrow("frame.contentWindow.event = 1;", '"SecurityError: Blocked a frame with origin \\"suborigin+foobar+http://127.0.0.1:8000\\" from accessing a cross-origin frame."');
            finishJSTest();
        };
    </script>
</body>
</html>
