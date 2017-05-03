
// JavaScript for SearchBooks Demo
// Jim Skon, Kenyon College, 2017
$(document).ready(function () {

    $("#search-btn").click(getMatches);

    $("#clear").click(clearResults);

    $('#showLinesButton').click( function () {
	if ($(this).attr("state")=="off") {
	    $(this).attr("state", "on");
	    $(this).text("Show Matches");
	} else {
	    $(this).attr("state","off");	    
	    $(this).text("Hide Matches");
	}
    });
    
    $(document).ajaxStart(function () {
	$("#loading").show();
    }).ajaxStop(function () {
	$("#loading").hide();
    });

});

function processResults(results) {
    $('#searchresults').empty();
    $('#searchresults').append(results);
}

function clearResults() {
    $('#searchresults').empty();
}

function getMatches(){
    if ($('#search').val().length < 3) return;
    $('#searchresults').empty();
    $.ajax({
		url: '/cgi-bin/skon_SearchBooks.cgi?term='+$('#search').val()+'&time='+$('#wait_time').val()+'&showmatches='+$('#showLinesButton').attr('state'),
		dataType: 'text',
		success: processResults,
		error: function(){alert("Error: Something went wrong");}
    });

}
