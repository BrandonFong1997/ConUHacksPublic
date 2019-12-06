:: Emails a warning out
:: Usage: > email.bat <mail address to send email to>

cmail.exe -host:hacks@joelruhland.net:password@smtp.zoho.com:587 -starttls -from:hacks@joelruhland.net -to:%1 -subject:"Urgent Alert!" -body:"User has fallen!"
