(defun what-line ()
  "Print the current buffer line number and narrowed line number of point."
  (interactive) 
  (let ((start (point-min))
	(n (line-number-at-pos))
	(total (count-lines (point-min) (point-max))))
	(save-excursion
		(goto-char (point-max))
		(setq actual (if (and (/= (point-min) (point-max))
                       (not (bolp)))
                  (1- total)
                total)))
    (if (= start 1)
	(message "Line %d of %d" n actual)
      (save-excursion
	(save-restriction
	  (widen)
	  (save-excursion
		(goto-char (point-max))
		(setq widtotal (if (and (/= (point-min) (point-max))
                       (not (bolp)))
                  (1- (count-lines (point-min) (point-max)))
                (count-lines (point-min) (point-max)))))
	  (message "line %d of %d (narrowed line %d of %d)"
		   (+ n (line-number-at-pos start) -1) widtotal n actual))))))



