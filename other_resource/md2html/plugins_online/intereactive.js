function mouseShapeChange(event)
{
    if(Math.abs(event.pageX - spliter_position) < 8)
    {
        $(this).css("cursor", "e-resize");
    }
}

function forbidden_select(element)
{
    element.css("-webkit-user-select", "none");
    element.css("-moz-user-select", "none");
    element.css("-ms-user-select", "none");
    element.css("user-select", "none");
}

function allow_select(element)
{
    element.css("-webkit-user-select", "text");
    element.css("-moz-user-select", "text");
    element.css("-ms-user-select", "text");
    element.css("user-select", "text");
}

function select_li(element, is_sticked)
{
    $("ul.index li a").css({"background-color": "transparent", "border-color": "transparent"}); // other
    $("ul.index li a").data("selected", "false");
    $("ul.index li div").css("border-color", "rgb(150, 150, 150)");
    
    element.css({"background-color": "#CCE8FF", "border-color": "#99D1FF"}); // selected
    element.data("selected", "true");
    var parent_color = element.parent("li").children("div").css("border-color");
    element.parents("li").children("div").css("border-color", "#3574F0");
    element.parent("li").children("div").css("border-color", parent_color);

    if(is_sticked)
    {
        return;
    }

    element.parents("ul").show();
    var parent_transform = element.parent("li").children("div").css("transform");
    
    element.parents("li").children("div").css("transform", "rotate(45deg)");
    element.parent("li").children("div").css("transform", parent_transform);
}

function root_show_all(element)
{
    element.find("ul").show();
    element.find("div").css("transform", "rotate(45deg)");
}

function root_hide_all(element)
{
    element.find("ul").hide();
    element.find("div").css("transform", "rotate(-45deg)");
}

function arrow_show_all(element)
{
    var father = element.parent();
    father.find("ul").show();
    element.css("transform", "rotate(45deg)");
    father.find("div").css("transform", "rotate(45deg)");
}

function arrow_hide_all(element)
{
    var father = element.parent();
    father.find("ul").hide();
    element.css("transform", "rotate(-45deg)");
    father.find("div").css("transform", "rotate(-45deg)");
}

function split2(main_div, left_div, right_div, user_config = {})
{
    var config = 
    {
        "gap_width": 0,
        "left_width": 250,
        "left_min_width": 0,
        "left_max_width": 1000,
    }
    config = {...config, ...user_config};

    left_div.css("float", "left");
    left_div.outerWidth(config.left_width);
    right_div.css("width", "auto");
    
    var spliter_position = left_div.offset().left + left_div.outerWidth() + config.gap_width/2;
    var in_range = false;
    var left_pressed = false;
    var last_x = 0;
    var right_width_before_hide = 0;

    $("html").mousedown(
        function(event)
        {
            left_pressed = true;
            last_x = event.pageX;
        }
    );
    $("html").mouseup(
        function(event)
        {
            left_pressed = false;
        }
    );
    $("html").mousemove(
        function(event)
        {
            if(Math.abs(event.pageX - spliter_position) < 8)
            {
                if(!in_range)
                {
                    $(this).css("cursor", "e-resize");
                    in_range = true;
                }
            }
            else
            {
                if(in_range && !left_pressed)
                {
                    $(this).css("cursor", "auto");
                    in_range = false;
                }
            }

            if(in_range && left_pressed)
            {
                forbidden_select(left_div);
                forbidden_select(right_div);
                var current_x = event.pageX;
                var dx = current_x-last_x;

                var left_target_width = event.pageX - left_div.position().left;
                // var left_target_width = left_div.outerWidth()+dx;
               
            	if(left_target_width < config.left_min_width)
                {
                    left_target_width = config.left_min_width;
                }
                else if(left_target_width > config.left_max_width)
                {
                	left_target_width = config.left_max_width;
                }
      
                left_div.outerWidth(left_target_width);
                
                last_x = current_x;
                spliter_position = left_div.offset().left + left_div.outerWidth() + config.gap_width/2;
            }
            else
            {
                allow_select(left_div);
                allow_select(right_div);
            }
        }
    );
    left_div.prepend("<div id=\"stick-button\" class=\"stick\"></div>");
    $("#stick-button").click(
        function(event)
        {
            is_sticked = !is_sticked;
            if(is_sticked)
            {
                $(this).attr("class", "stick-clicked");
            }
            else
            {
                $(this).attr("class", "stick");
            }
        }
    );
    $("#stick-button").mouseover(
        function(event)
        {
            $(this).attr("class", "stick-hover");
        }
    );
    $("#stick-button").mouseout(
        function(event)
        {
            if(is_sticked)
            {
                $(this).attr("class", "stick-clicked");
            }
            else
            {
                $(this).attr("class", "stick");
            }
        }
    );

    $(".item-arrow").each(
    	function(element)
    	{
    		if($(this).parent().has("ul").length)
    		{
    			$(this).parent().children("ul").hide();
    			$(this).css("visibility", "visible");
    		}
    	}
    );

    $(".item-arrow").click(
    	function(event)
    	{
            event.stopPropagation();
            if(event.altKey)
            {
                if($(this).parent().children("ul").css("display") != "none")
                {
                    arrow_hide_all($(this));
                }
                else
                {
                    arrow_show_all($(this));
                }
            }
            else
            {
        		if($(this).parent().children("ul").css("display") != "none")
        		{
        			$(this).css("transform", "rotate(-45deg)");
        			$(this).parent().children("ul").hide();
        		}
        		else
        		{
        			$(this).css("transform", "rotate(45deg)");
        			$(this).parent().children("ul").show();
        		}
            }
    	}
    );

    var hidden_all = true;
    left_div.click(
        function(event)
        {
            if(event.altKey)
            {
                $("ul.index").children("li").children("ul").each(
                    function(event)
                    {
                        if($(this).css("display") != "none")
                        {
                            hidden_all = false;
                        }
                    }
                );
                if(hidden_all)
                {
                    root_show_all($(this).children("ul"));
                }
                else
                {
                    root_hide_all($(this).children("ul"));
                }
                hidden_all = true;
            }
        }
    );

    $("ul.index li a").mouseover(
    	function(event)
    	{
    		if($(this).data("selected") == "true") // selected
    		{
    			return;
    		}

    		$(this).css("background-color", "#E5F3FF"); // hover
    	}
    );
    $("ul.index li a").mouseout(
    	function(event)
    	{
    		if($(this).data("selected") == "true") // selected
    		{
    			return;
    		}

    		$(this).css("background-color", "transparent"); // not hover
    	}
    );
    

    var all_hi = [];
    $("ul.index li a").each(
        function()
        {
            all_hi.push($(this).attr("href"));
        }
    );

    var clicking = false;
    var is_sticked = false;
    right_div.scroll(
        function(event)
        {
            if(clicking)
            {
                return;
            }

            var current_sec = all_hi[all_hi.length-1];
            for(var i = 0; i < all_hi.length; i++)
            {
                if($(all_hi[i]).offset().top-20 > 0)
                {
                    current_sec = all_hi[Math.max(i-1, 0)];
                    break;
                }
            }
            select_li($("ul.index li a[href=\""+current_sec+"\"]"), is_sticked);
        }
    );

    $("ul.index li a").click(
        function(event)
        {
            clicking = true;
            event.preventDefault();
            select_li($(this), is_sticked);
            right_div.animate({scrollTop: right_div.scrollTop() + $($(this).attr("href")).offset().top - 10}, 200, function(){clicking=false;});
        }
    );

    $(".h-title").click(
        function(event)
        {
            var stick_status = is_sticked;
            is_sticked = false;
            select_li($("ul.index li a[href=\"#"+$(this).attr("id")+"\"]"), is_sticked);
            is_sticked = stick_status;
        }
    );
}

$(
    function()
    {
        $(".markdown-body a").attr("target", "_blank");
        $(".markdown-body a").attr("rel", "noopener noreferrer");
        
        split2($("#all"), $("#content-index"), $("#content-container"));
    }
);