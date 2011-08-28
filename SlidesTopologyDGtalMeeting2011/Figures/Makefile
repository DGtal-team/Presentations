FIG2PNG=\
	suivi-artzy.fig \
	IntAdjacency.fig \
	SurfaceTracking.fig \
	SurfaceTracking2.fig \
	fig-boundary.fig \
	Boundaries.fig \
	BdryOp-0.fig \
	BdryOp-1.fig \
	BdryOp-2.fig \
	BdryOp-3.fig \
	BdryOp-4.fig \
	BdryOp-5.fig \
	BdryOp-6.fig \
	RegularGrid2.fig \
	KhalimskyIsomorphism.fig

PNGFILES=${FIG2PNG:.fig=.png}
FIG2PNGCMD=fig2dev -L png -F -m 1

all: fig2png

fig2png: 
	@for i in ${FIG2PNG}; do \
		j=`echo $$i | sed 's/.fig/.png/'`; \
		echo "$$i $$j"; \
		${FIG2PNGCMD} $$i $$j; \
	done

clean:
	rm -f ${PNGFILES}
