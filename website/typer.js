// from: https://hackernoon.com/how-to-make-a-terminal-like-portfolio-website-for-yourself-27d7a7030004

var Typer = {
	text: "",
	accessCountimer:null,
	index: 0, 
	speed: 2,
	file: "", 
	accessCount: 0,
	deniedCount: 0, 
	init: function() {
		accessCountimer = setInterval(
            function() {
                Typer.updLstChr();
            },
            500
        ); 
		$.get(Typer.file, function(data) {
			Typer.text = data;
			Typer.text = Typer.text.slice(0, Typer.text.length - 1);
		});
	},
 
	content: function() {
		return $("#console").html();
	},
 
	write: function(str) {
		$("#console").append(str);
		return false;
	},
 
	addText: function(key) {
		
		if (key.keyCode == 18) {
			Typer.accessCount++; 
			
			if (Typer.accessCount >= 3)
				Typer.makeAccess(); 

		} else if (key.keyCode == 20) {
			Typer.deniedCount++; 
			
			if (Typer.deniedCount >= 3)
				Typer.makeDenied(); 

		} else if (key.keyCode == 27) { 
			Typer.hidepop(); 

		} else if (Typer.text) { 
			var cont = Typer.content(); 

			if (cont.substring(cont.length - 1, cont.length) == "|") 
				$("#console").html($("#console").html().substring(0, cont.length - 1)); 
			if (key.keyCode != 8) { 
				Typer.index += Typer.speed;	
            } else {
			    if (Typer.index > 0) 
				    Typer.index -= Typer.speed;
			}

			var text = Typer.text.substring(0,Typer.index);
			var rtn = new RegExp("\n", "g"); 
	
			$("#console").html(text.replace(rtn, "<br/>"));
			window.scrollBy(0,50); 
		}
		
		if (key.preventDefault && key.keyCode != 122)
			key.preventDefault()
		
		if(key.keyCode != 122)
			key.returnValue = false;

	},
 
	updLstChr: function(){ 
		var cont = this.content(); 

		if (cont.substring(cont.length - 1,cont.length) == "|") {
			$("#console").html($("#console").html().substring(0, cont.length - 1)); 
        } else {
			this.write("|");
        }
	}
}
