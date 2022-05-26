// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright 2018 Alexandre Macabies

function flip_dogo() {
    let $elem = $(".easter-egg-42");

    if ($elem.hasClass("right"))
        $elem.removeClass("right");
    else
        $elem.addClass("right");
}

function plan_42_event(callback) {
    const wait_for = (102 - (new Date()).getSeconds()) % 60;

    window.setTimeout(
        () => {
            callback();
            plan_42_event(callback);
        },
        1000 * wait_for
    );
}

$(function() {
    $('[role=complementary] h1').text('').html('<img src="/static/img/logo.png" style="width:100%; padding: 20px 10px;" />');

    // let's have some easter egg
    const p1 = $('<img src="/static/img/old.apng" class="easter-egg" />'),
        p2 = $('<img src="/static/img/duck.apng" class="easter-egg" />'),
        dog = $('<div class="easter-egg-42"><img src="/static/img/dogo.gif" class="dogo" /></div>');

    plan_42_event(flip_dogo);

    $('body').append(p1).append(p2).append(dog);
    $('.easter-egg').on('click', function() {
        const p = $(this);
        if (p.hasClass('visible')) {
            p.removeClass('visible');
        } else {
            p.removeClass('peak').addClass('visible');
        }
    });

    [p1, p2].forEach(function (p) {
        if (Math.random() < .5) {
            p.css('left', 64 + Math.random() * (document.body.clientWidth - 128 - 64));
            setTimeout(function() {
                const p = Math.random() < .5 ? p1 : p2;
                p.addClass('peak');
            },
                1000 * (5 + Math.random() * 10));
        }
    });
});
