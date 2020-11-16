(defun delete-horizontal-space (&optional backward-only forward-only)
  "Delete all spaces and tabs around point.
If BACKWARD-ONLY is non-nil, delete them only before point."
  (interactive "*P")
  (let ((orig-pos (point)))
    (delete-region
     (if backward-only
	 orig-pos
       (progn
	 (skip-chars-forward " \t")
	 (constrain-to-field nil orig-pos t)))
     (if forward-only
	 orig-pos
       (progn
	 (skip-chars-backward " \t")
	 (constrain-to-field nil orig-pos t))))))
