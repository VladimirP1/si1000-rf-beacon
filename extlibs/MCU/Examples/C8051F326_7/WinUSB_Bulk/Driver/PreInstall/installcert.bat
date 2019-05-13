@echo off

call certutil -addstore "TrustedPublisher" silabs.cer
if %ERRORLEVEL% neq 0 goto :ERROR_FAIL

goto:eof

:ERROR_FAIL
echo Failed to install certificate
exit /b -1
