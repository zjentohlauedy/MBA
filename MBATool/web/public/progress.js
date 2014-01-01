window._progress = window._progress || {};

var _progress = window._progress;

_progress.stage = 1;

$(window).load( function() {
    $("ol.progress").each( function() {
        _progress.stages = $(this).children("li").length;

        $(this).attr( "data-progress-steps", _progress.stages);

        $(this).children("li").each( function() {
            $(this).addClass("progress-todo");
        });
    });

    $("button.next").click( function() {
        if ( _progress.stage > _progress.stages ) {
            $("ol.progress").each( function() {
                $(this).children("li").each( function() {
                    $(this).removeClass("progress-done");
                    $(this).addClass("progress-todo");
                });
            });

            _progress.stage = 1;

            return;
        }

        var item = $("ol.progress li:nth-child(" + _progress.stage + ")")

        item.removeClass("progress-todo");
        item.addClass("progress-done");

        _progress.stage++;
    });
})
