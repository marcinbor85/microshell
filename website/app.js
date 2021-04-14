// from: https://hackernoon.com/how-to-make-a-terminal-like-portfolio-website-for-yourself-27d7a7030004

Typer.speed = 3;
Typer.file = "microshell.html";
Typer.init();
 
var timer = setInterval("t();", 10);

function t() {
	Typer.addText({"keyCode": 123748});
	
	if (Typer.index > Typer.text.length) {
		clearInterval(timer);
	}
}
